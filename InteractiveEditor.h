#pragma once
#include <iostream>
#include <string>
#include <Windows.h> // 用于控制台操作
#include <conio.h> // 对于_getch()
#include <sstream> // 对于 std::ostringstream
#include <iomanip> // 对于 std::setw 和 std::setfill
#include <algorithm>
#include "TextEditor.h"
#define INSERT_MODE 1
using std::min;

enum class Mode {
    NORMAL,
    INSERT,
    COMMAND
};


class InteractiveEditor :public TextEditor {
private:
    /*
    注意，cursor的设计是按照光标相对于整个文本的绝对位置来设计的
    另外需要变量或者其他方法来表示光标在当前的位置
    */
    int cursorX = 0; // 光标的X位置
    int cursorY = 0; // 光标的Y位置
    int visibleStart; // 当前可视的文本的行绝对位置起始索引
    int visibleEnd;  // 当前可视的文本的行绝对位置结束索引
    Mode mode;
    std::string filename;

public:
    InteractiveEditor();
    InteractiveEditor(const std::string& filename);

    // 清屏
    void clearScreen();
    void clearScreenExceptStatusBar();

    // 移动光标到指定位置
    void moveCursor(int x, int y);

    // 绘制状态栏及文本
    void updateStatusBar();
    void drawText();

    // 文本滚动
    bool scrollIfNeeded();

    // 计算工具
    int calculatePercent();
    int getConsoleY();

    // 处理键盘输出逻辑
    void handleKeyboardInput();

    // 光标移动逻辑
    void moveCursorUp();
    void moveCursorDown();
    void moveCursorLeft();
    void moveCursorRight();

    // 启动编辑器逻辑
    void run();
};


