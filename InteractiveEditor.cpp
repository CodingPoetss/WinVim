#include"InteractiveEditor.h"
#include <algorithm>


// ���ߺ���
void toLastCursorInTab(int& realPos) {
    int distanceToNextTabStop = TABWIDTH - ((realPos + 1) % TABWIDTH);
    realPos += distanceToNextTabStop; // ������ƶ�����ǰ�����Ʊ�λ�����һ��
}

void backLastCursorInTab(int& realPos) {
    int distanceToNextTabStop = TABWIDTH - ((realPos + 1) % TABWIDTH);
    realPos -= distanceToNextTabStop; // ������ƶ�����ǰ�����Ʊ�λ�����һ��
}

int getConsoleWidth() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int columns;

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;

    return columns;
}

int getConsoleHeight() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int rows;

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

    return rows;
}

int getVisibleLines() {
    // ����״̬��ռ��1��
    return getConsoleHeight() - 1;
}

// ���ع��
void hideCursor() {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 20; // ����С������Ҫ����Ϊ���ǽ����ع�꣩
    info.bVisible = FALSE; // ���ù��Ϊ���ɼ�
    SetConsoleCursorInfo(consoleHandle, &info);
}

// ��ʾ���
void showCursor() {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 20; // ����С������ΪĬ��ֵ������Ҫ���κ�ֵ��
    info.bVisible = TRUE; // ���ù��Ϊ�ɼ�
    SetConsoleCursorInfo(consoleHandle, &info);
}

void limitConsoleView() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD newBufferSize;
    newBufferSize.X = getConsoleWidth(); // ����̨���
    newBufferSize.Y = getConsoleHeight(); // ����̨�߶ȣ�����Ϊ����ͼ���ڵĸ߶���ͬ

    SetConsoleScreenBufferSize(hConsole, newBufferSize);
}

InteractiveEditor::InteractiveEditor() {
    filename = "test.txt";
    visibleStart = 0;
    visibleEnd = getVisibleLines();
    mode = Mode::NORMAL;
}

InteractiveEditor::InteractiveEditor(const std::string& filename) {
    this->filename = filename;
    visibleStart = 0;
    visibleEnd = getVisibleLines();
    mode = Mode::NORMAL;
}


// ����
void InteractiveEditor::clearScreen() {
    system("cls");
}

void InteractiveEditor::clearScreenExceptStatusBar() {
    int totalLines = getVisibleLines(); // ��ȡ��״̬����Ŀɼ�����
    hideCursor();
    for (int i = 0; i < totalLines; ++i) { // �����ײ�״̬��
        moveCursor(0, i);
        std::cout << std::string(getConsoleWidth(), ' '); // ʹ�ÿո񸲸Ǹ���
    }
    moveCursor(0, 0); // ��������õ���Ļ����
    showCursor();
}

// �ƶ���굽ָ��λ��
void InteractiveEditor::moveCursor(int x, int y) {
    COORD coord{};
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void InteractiveEditor::updateStatusBar() {
    int width = getConsoleWidth() - 2; // ����һЩ�߾�
    std::ostringstream statusStream;

    // ����״̬���ı�
    statusStream << (mode == Mode::INSERT ? "-- INSERT --" : "");
    statusStream << std::right << std::setw(width - (mode==Mode::NORMAL?20:32)); // �Ҷ���ʣ�ಿ��
    statusStream << "Line " << cursorX << ", Col " << cursorY;
    statusStream << std::right << std::setw(10) << calculatePercent() << "%";

    // ��ȡ״̬���ַ���
    std::string status = statusStream.str();
    // ����״̬����������Ӧ����̨���
    status = status.substr(0, width);

    hideCursor();
    // ��λ���ײ�״̬��λ�ò���ӡ״̬��Ϣ
    moveCursor(0, getConsoleHeight() - 1); // ��λ���ײ���
    std::cout << std::string(width, ' ');
    moveCursor(0, getConsoleHeight() - 1); // ��λ���ײ���
    std::cout << status;
    moveCursor(cursorX, getConsoleY());
    showCursor();
}

bool InteractiveEditor::scrollIfNeeded() {
    int screenHeight = getVisibleLines(); // ��ȡ�ɼ�������Ԥ��һ�и�״̬��
    int upperMargin = 5; // �ϱ߾���ֵ
    int lowerMargin = screenHeight - 6; // �±߾���ֵ������״̬��

    // ���¹����߼�
    if (getConsoleY() > lowerMargin && visibleEnd < getRows()) {
        // ���ӽ��ײ���δ�����ı��ײ�
        visibleStart++;
        visibleEnd++;
        drawText(); // �ػ��ı�
        return true;
    }
    // ���Ϲ����߼�
    else if (getConsoleY() < upperMargin && visibleStart > 0) {
        // ���ӽ�������δ�����ı�����
        visibleStart--;
        visibleEnd--;
        drawText(); // �ػ��ı�
        return true;
    }
    return false;
}

void InteractiveEditor::drawText() {
    hideCursor(); // �ڻ����ı�ǰ���ع��
    moveCursor(0, 0);
    for (int i = visibleStart,j=0; i < visibleEnd; i++,j++) {
        std::cout << std::string(getConsoleWidth(), ' ');
        moveCursor(0, j);
        getCurrentBuffer(i).print();
    }
    moveCursor(cursorX, getConsoleY());
    showCursor(); // ����ı����ƺ���ʾ���
}

int InteractiveEditor::calculatePercent() {
    double percent = (double)(cursorY+1) / getRows() * 100;
    return static_cast<int>(percent);
}

int InteractiveEditor::getConsoleY(){
    return cursorY - visibleStart;  // ��ȡ��ǰ����ڿ���̨�����������
}

void InteractiveEditor::handleKeyboardInput() {
    int ch; // ʹ�� int ������ char ���洢 _getch() �ķ���ֵ
    while (true) {
        if (_kbhit()) { // �������Ƿ�������
            ch = _getch(); // ��ȡ�����ַ��������Ե�����̨
            if (ch == 0 || ch == 224) { // ��⵽���ⰴ���ı��
                ch = _getch(); // ��ȡ���ⰴ����ʵ�ʰ�������
                switch (ch) {
                case 72: // �ϼ�ͷ
                    moveCursorUp();
                    updateStatusBar();
                    break;
                case 80: // �¼�ͷ
                    moveCursorDown();
                    updateStatusBar();
                    break;
                case 75: // ���ͷ
                    moveCursorLeft();
                    updateStatusBar();
                    break;
                case 77: // �Ҽ�ͷ
                    moveCursorRight();
                    updateStatusBar();
                    break;
                }
            }
            else {
                switch (ch) {
                case 27: // ESC ��
                    mode = Mode::NORMAL;
                    updateStatusBar();
                    break;
                case 105: // ����i
                    mode = Mode::INSERT;
                    updateStatusBar();
                    break;
                }
            }
        }

    }
}

/*
�����߼���ֻ��Ҫһ��cursor���꼴��
��ͨ��getCurrentChar(int cursorX)����ȡ��ǰgapbuffer�е��ַ�
*/
void InteractiveEditor::moveCursorUp() {
    if (cursorY > 0) {
        cursorY--;
        if (cursorX > getCurrentLen(cursorY) - 1) {
            cursorX = getCurrentLen(cursorY) - 1;
        }
        cursorX = handleTabCollision(cursorY, cursorX);

        //����ǻ��з���ǿ�ƹ��ֻ���ڵ�һ��
        if (isNewline(cursorY))cursorX = 0;
    }
    if (scrollIfNeeded()) {
        moveCursor(cursorX, getConsoleY() + 1); // ʹ�����λ��
    }
    moveCursor(cursorX, getConsoleY());
}

void InteractiveEditor::moveCursorDown() {
    // ��Ҫ����Ƿ񳬳��ı��ײ�
    if (cursorY < getRows() - 1) {
        cursorY++;
        if (cursorX > getCurrentLen(cursorY) - 1) {
            cursorX = getCurrentLen(cursorY) - 1;
        }
        cursorX = handleTabCollision(cursorY, cursorX);

        //����ǻ��з���ǿ�ƹ��ֻ���ڵ�һ��
        if (isNewline(cursorY))cursorX = 0;
    }
    if (scrollIfNeeded()) {
        moveCursor(cursorX, getConsoleY() -1); // ʹ�����λ��
        return;
    }
    moveCursor(cursorX, getConsoleY());
}

void InteractiveEditor::moveCursorLeft() {
    int lastCursor = getLastCursor(cursorY, cursorX);
    if (lastCursor >= 0) {
        cursorX = lastCursor;
        getCurrentBuffer(cursorY).moveCurrentGapLeft();
    }
    moveCursor(cursorX, getConsoleY());
}

void InteractiveEditor::moveCursorRight() {
    int nextCursor = getNextCursor(cursorY, cursorX);
    // �ж���һ�����λ���Ƿ񳬳����������
    if (nextCursor <= getCurrentLen(cursorY) - 1) {
        cursorX = nextCursor;
        // ����ȷ�����Ƿ����ƶ����߼���ꣿ
        getCurrentBuffer(cursorY).moveCurrentGapRight();
    }
    moveCursor(cursorX, getConsoleY());
}


// ���������ı��༭�����߼�
void InteractiveEditor::run() {
    /*clearScreen();*/
    loadFile(filename);
    drawText();
    /*display();*/
    if (getCurrentBuffer(cursorY)[0] == '\t')cursorX += 7;      //�ж��Ƿ�һ��ʼ����\t��
    moveCursor(cursorX, cursorY);
    limitConsoleView();
    updateStatusBar();
    handleKeyboardInput();
}
