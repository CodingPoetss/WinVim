#include <iostream>
#include <conio.h> // 对于_getch()
#include <vector>
#include <string>
#include <Windows.h> // 用于控制台操作
#include"TextEditor.cpp"

void toLastCursorInTab(int& realPos) {
    int distanceToNextTabStop = TABWIDTH - ((realPos + 1) % TABWIDTH);
    realPos += distanceToNextTabStop; // 将光标移动到当前所属制表位的最后一格
}

void backLastCursorInTab(int& realPos) {
    int distanceToNextTabStop = TABWIDTH - ((realPos + 1) % TABWIDTH);
    realPos -= distanceToNextTabStop; // 将光标移动到当前所属制表位的最后一格
}

class InteractiveEditor : public TextEditor {
private:
    int cursorX = 0; // 光标的X位置
    int cursorY = 0; // 光标的Y位置
    /*int bufferX = 0;
    int bufferY = 0;*/
    std::string filename;

public:
    InteractiveEditor(const std::string& filename = "undefine") {
        this->filename = filename;
    }


    // 清屏
    void clearScreen() {
        system("cls");
    }

    // 移动光标到指定位置
    void moveCursor(int x, int y) {
        COORD coord;
        coord.X = x;
        coord.Y = y;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    }

    void handleKeyboardInput() {
        int ch; // 使用 int 而不是 char 来存储 _getch() 的返回值
        while (true) {
            if (_kbhit()) { // 检查键盘是否有输入
                ch = _getch(); // 获取输入字符，不回显到控制台
                if (ch == 0 || ch == 224) { // 检测到特殊按键的标记
                    ch = _getch(); // 获取特殊按键的实际按键代码
                    switch (ch) {
                    case 72: // 上箭头
                        moveCursorUp();
                        break;
                    case 80: // 下箭头
                        moveCursorDown();
                        break;
                    case 75: // 左箭头
                        moveCursorLeft();
                        break;
                    case 77: // 右箭头
                        moveCursorRight();
                        break;
                    }
                }
                else {
                    switch (ch) {
                    case 27: // ESC 键
                        return; // 退出编辑器
                        // 处理其他非特殊按键...


                    }
                }
            }
        }
    }

    /*
    更新逻辑，只需要一个cursor坐标即可
    我通过getCurrentChar(int cursorX)来获取当前gapbuffer中的字符
    */
    void moveCursorUp() {
        if (cursorY > 0) {
            cursorY--;
            if (cursorX > getCurrentLen(cursorY) - 1) {
                cursorX = getCurrentLen(cursorY) - 1;
            }
            cursorX = handleTabCollision(cursorY, cursorX);

            //如果是换行符，强制光标只能在第一格
            if (isNewline(cursorY))cursorX = 0;
        }
        moveCursor(cursorX, cursorY);
    }

    void moveCursorDown() {
        // 需要检查是否超出文本底部
        if (cursorY < getRows() - 1) {
            cursorY++;
            if (cursorX > getCurrentLen(cursorY)-1) {
                cursorX = getCurrentLen(cursorY) - 1;
            }
            cursorX = handleTabCollision(cursorY,cursorX);

            //如果是换行符，强制光标只能在第一格
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
        // 判断下一个光标位置是否超出光标物理长度
        if (nextCursor <= getCurrentLen(cursorY)-1) {
            cursorX = nextCursor;
            // 后续确保：是否是移动的逻辑光标？
            getCurrentBuffer(cursorY).moveCurrentGapRight();
        }
        moveCursor(cursorX, cursorY);
    }


    // 启动进入文本编辑器的逻辑
    void run() {
        clearScreen();
        loadFile(filename);
        display();
        if (getCurrentBuffer(cursorY)[0] == '\t')cursorX += 7;      //判断是否一开始就在\t上
        moveCursor(cursorX, cursorY);
        handleKeyboardInput();
    }
};

