#include <iostream>
#include <conio.h> // ����_getch()
#include <vector>
#include <string>
#include <Windows.h> // ���ڿ���̨����
#include"TextEditor.cpp"

void toLastCursorInTab(int& realPos) {
    int distanceToNextTabStop = TABWIDTH - ((realPos + 1) % TABWIDTH);
    realPos += distanceToNextTabStop; // ������ƶ�����ǰ�����Ʊ�λ�����һ��
}

void backLastCursorInTab(int& realPos) {
    int distanceToNextTabStop = TABWIDTH - ((realPos + 1) % TABWIDTH);
    realPos -= distanceToNextTabStop; // ������ƶ�����ǰ�����Ʊ�λ�����һ��
}

class InteractiveEditor : public TextEditor {
private:
    int cursorX = 0; // ����Xλ��
    int cursorY = 0; // ����Yλ��
    /*int bufferX = 0;
    int bufferY = 0;*/
    std::string filename;

public:
    InteractiveEditor(const std::string& filename = "undefine") {
        this->filename = filename;
    }


    // ����
    void clearScreen() {
        system("cls");
    }

    // �ƶ���굽ָ��λ��
    void moveCursor(int x, int y) {
        COORD coord;
        coord.X = x;
        coord.Y = y;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    }

    void handleKeyboardInput() {
        int ch; // ʹ�� int ������ char ���洢 _getch() �ķ���ֵ
        while (true) {
            if (_kbhit()) { // �������Ƿ�������
                ch = _getch(); // ��ȡ�����ַ��������Ե�����̨
                if (ch == 0 || ch == 224) { // ��⵽���ⰴ���ı��
                    ch = _getch(); // ��ȡ���ⰴ����ʵ�ʰ�������
                    switch (ch) {
                    case 72: // �ϼ�ͷ
                        moveCursorUp();
                        break;
                    case 80: // �¼�ͷ
                        moveCursorDown();
                        break;
                    case 75: // ���ͷ
                        moveCursorLeft();
                        break;
                    case 77: // �Ҽ�ͷ
                        moveCursorRight();
                        break;
                    }
                }
                else {
                    switch (ch) {
                    case 27: // ESC ��
                        return; // �˳��༭��
                        // �������������ⰴ��...


                    }
                }
            }
        }
    }

    /*
    �����߼���ֻ��Ҫһ��cursor���꼴��
    ��ͨ��getCurrentChar(int cursorX)����ȡ��ǰgapbuffer�е��ַ�
    */
    void moveCursorUp() {
        if (cursorY > 0) {
            cursorY--;
            if (cursorX > getCurrentLen(cursorY) - 1) {
                cursorX = getCurrentLen(cursorY) - 1;
            }
            cursorX = handleTabCollision(cursorY, cursorX);

            //����ǻ��з���ǿ�ƹ��ֻ���ڵ�һ��
            if (isNewline(cursorY))cursorX = 0;
        }
        moveCursor(cursorX, cursorY);
    }

    void moveCursorDown() {
        // ��Ҫ����Ƿ񳬳��ı��ײ�
        if (cursorY < getRows() - 1) {
            cursorY++;
            if (cursorX > getCurrentLen(cursorY)-1) {
                cursorX = getCurrentLen(cursorY) - 1;
            }
            cursorX = handleTabCollision(cursorY,cursorX);

            //����ǻ��з���ǿ�ƹ��ֻ���ڵ�һ��
            if (isNewline(cursorY))cursorX = 0;
        }
        moveCursor(cursorX, cursorY);
    }

    void moveCursorLeft() {
        int lastCursor = getLastCursor(cursorY, cursorX);
        if (lastCursor >= 0) {
            cursorX = lastCursor;
            getCurrentBuffer(cursorY).moveCurrentGapLeft();
        }
        moveCursor(cursorX, cursorY);
    }

    void moveCursorRight() {
        int nextCursor = getNextCursor(cursorY, cursorX);
        // �ж���һ�����λ���Ƿ񳬳����������
        if (nextCursor <= getCurrentLen(cursorY)-1) {
            cursorX = nextCursor;
            // ����ȷ�����Ƿ����ƶ����߼���ꣿ
            getCurrentBuffer(cursorY).moveCurrentGapRight();
        }
        moveCursor(cursorX, cursorY);
    }


    // ���������ı��༭�����߼�
    void run() {
        clearScreen();
        loadFile(filename);
        display();
        if (getCurrentBuffer(cursorY)[0] == '\t')cursorX += 7;      //�ж��Ƿ�һ��ʼ����\t��
        moveCursor(cursorX, cursorY);
        handleKeyboardInput();
    }
};

