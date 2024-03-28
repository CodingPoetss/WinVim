#include"InteractiveEditor.h"
#include <algorithm>


// 工具函数
void toLastCursorInTab(int& realPos) {
    int distanceToNextTabStop = TABWIDTH - ((realPos + 1) % TABWIDTH);
    realPos += distanceToNextTabStop; // 将光标移动到当前所属制表位的最后一格
}

void backLastCursorInTab(int& realPos) {
    int distanceToNextTabStop = TABWIDTH - ((realPos + 1) % TABWIDTH);
    realPos -= distanceToNextTabStop; // 将光标移动到当前所属制表位的最后一格
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
    // 假设状态栏占用1行
    return getConsoleHeight() - 1;
}

// 隐藏光标
void hideCursor() {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 20; // 光标大小（不重要，因为我们将隐藏光标）
    info.bVisible = FALSE; // 设置光标为不可见
    SetConsoleCursorInfo(consoleHandle, &info);
}

// 显示光标
void showCursor() {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 20; // 光标大小（设置为默认值或你想要的任何值）
    info.bVisible = TRUE; // 设置光标为可见
    SetConsoleCursorInfo(consoleHandle, &info);
}

void limitConsoleView() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD newBufferSize;
    newBufferSize.X = getConsoleWidth(); // 控制台宽度
    newBufferSize.Y = getConsoleHeight(); // 控制台高度，设置为与视图窗口的高度相同

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


// 清屏
void InteractiveEditor::clearScreen() {
    system("cls");
}

void InteractiveEditor::clearScreenExceptStatusBar() {
    int totalLines = getVisibleLines(); // 获取除状态栏外的可见行数
    hideCursor();
    for (int i = 0; i < totalLines; ++i) { // 保留底部状态栏
        moveCursor(0, i);
        std::cout << std::string(getConsoleWidth(), ' '); // 使用空格覆盖该行
    }
    moveCursor(0, 0); // 将光标重置到屏幕顶部
    showCursor();
}

// 移动光标到指定位置
void InteractiveEditor::moveCursor(int x, int y) {
    COORD coord{};
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void InteractiveEditor::updateStatusBar() {
    int width = getConsoleWidth() - 2; // 留出一些边距
    std::ostringstream statusStream;

    // 构建状态栏文本
    statusStream << (mode == Mode::INSERT ? "-- INSERT --" : "");
    statusStream << std::right << std::setw(width - (mode==Mode::NORMAL?20:32)); // 右对齐剩余部分
    statusStream << "Line " << cursorX << ", Col " << cursorY;
    statusStream << std::right << std::setw(10) << calculatePercent() << "%";

    // 获取状态栏字符串
    std::string status = statusStream.str();
    // 限制状态栏长度以适应控制台宽度
    status = status.substr(0, width);

    hideCursor();
    // 定位到底部状态栏位置并打印状态信息
    moveCursor(0, getConsoleHeight() - 1); // 定位到底部行
    std::cout << std::string(width, ' ');
    moveCursor(0, getConsoleHeight() - 1); // 定位到底部行
    std::cout << status;
    moveCursor(cursorX, getConsoleY());
    showCursor();
}

bool InteractiveEditor::scrollIfNeeded() {
    int screenHeight = getVisibleLines(); // 获取可见行数，预留一行给状态栏
    int upperMargin = 5; // 上边距阈值
    int lowerMargin = screenHeight - 6; // 下边距阈值，考虑状态栏

    // 向下滚动逻辑
    if (getConsoleY() > lowerMargin && visibleEnd < getRows()) {
        // 光标接近底部且未到达文本底部
        visibleStart++;
        visibleEnd++;
        drawText(); // 重绘文本
        return true;
    }
    // 向上滚动逻辑
    else if (getConsoleY() < upperMargin && visibleStart > 0) {
        // 光标接近顶部且未到达文本顶部
        visibleStart--;
        visibleEnd--;
        drawText(); // 重绘文本
        return true;
    }
    return false;
}

void InteractiveEditor::drawText() {
    hideCursor(); // 在绘制文本前隐藏光标
    moveCursor(0, 0);
    for (int i = visibleStart,j=0; i < visibleEnd; i++,j++) {
        std::cout << std::string(getConsoleWidth(), ' ');
        moveCursor(0, j);
        getCurrentBuffer(i).print();
    }
    moveCursor(cursorX, getConsoleY());
    showCursor(); // 完成文本绘制后显示光标
}

int InteractiveEditor::calculatePercent() {
    double percent = (double)(cursorY+1) / getRows() * 100;
    return static_cast<int>(percent);
}

int InteractiveEditor::getConsoleY(){
    return cursorY - visibleStart;  // 获取当前光标在控制台的相对纵坐标
}

void InteractiveEditor::handleKeyboardInput() {
    int ch; // 使用 int 而不是 char 来存储 _getch() 的返回值
    while (true) {
        if (_kbhit()) { // 检查键盘是否有输入
            ch = _getch(); // 获取输入字符，不回显到控制台
            if (ch == 0 || ch == 224) { // 检测到特殊按键的标记
                ch = _getch(); // 获取特殊按键的实际按键代码
                switch (ch) {
                case 72: // 上箭头
                    moveCursorUp();
                    updateStatusBar();
                    break;
                case 80: // 下箭头
                    moveCursorDown();
                    updateStatusBar();
                    break;
                case 75: // 左箭头
                    moveCursorLeft();
                    updateStatusBar();
                    break;
                case 77: // 右箭头
                    moveCursorRight();
                    updateStatusBar();
                    break;
                }
            }
            else {
                switch (ch) {
                case 27: // ESC 键
                    mode = Mode::NORMAL;
                    updateStatusBar();
                    break;
                case 105: // 按键i
                    mode = Mode::INSERT;
                    updateStatusBar();
                    break;
                }
            }
        }

    }
}

/*
更新逻辑，只需要一个cursor坐标即可
我通过getCurrentChar(int cursorX)来获取当前gapbuffer中的字符
*/
void InteractiveEditor::moveCursorUp() {
    if (cursorY > 0) {
        cursorY--;
        if (cursorX > getCurrentLen(cursorY) - 1) {
            cursorX = getCurrentLen(cursorY) - 1;
        }
        cursorX = handleTabCollision(cursorY, cursorX);

        //如果是换行符，强制光标只能在第一格
        if (isNewline(cursorY))cursorX = 0;
    }
    if (scrollIfNeeded()) {
        moveCursor(cursorX, getConsoleY() + 1); // 使用相对位置
    }
    moveCursor(cursorX, getConsoleY());
}

void InteractiveEditor::moveCursorDown() {
    // 需要检查是否超出文本底部
    if (cursorY < getRows() - 1) {
        cursorY++;
        if (cursorX > getCurrentLen(cursorY) - 1) {
            cursorX = getCurrentLen(cursorY) - 1;
        }
        cursorX = handleTabCollision(cursorY, cursorX);

        //如果是换行符，强制光标只能在第一格
        if (isNewline(cursorY))cursorX = 0;
    }
    if (scrollIfNeeded()) {
        moveCursor(cursorX, getConsoleY() -1); // 使用相对位置
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
    // 判断下一个光标位置是否超出光标物理长度
    if (nextCursor <= getCurrentLen(cursorY) - 1) {
        cursorX = nextCursor;
        // 后续确保：是否是移动的逻辑光标？
        getCurrentBuffer(cursorY).moveCurrentGapRight();
    }
    moveCursor(cursorX, getConsoleY());
}


// 启动进入文本编辑器的逻辑
void InteractiveEditor::run() {
    /*clearScreen();*/
    loadFile(filename);
    drawText();
    /*display();*/
    if (getCurrentBuffer(cursorY)[0] == '\t')cursorX += 7;      //判断是否一开始就在\t上
    moveCursor(cursorX, cursorY);
    limitConsoleView();
    updateStatusBar();
    handleKeyboardInput();
}
