#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <fstream> // 用于文件操作
#include"GapBuffer.h"



class TextEditor {
private:
    std::vector<GapBuffer> lines; // 使用vector存储每行的GapBuffer对象

public:
    TextEditor();
    // 文件I/O操作
    void loadFile(const std::string& filename);
    void saveFile(const std::string& filename);

    // 用于展示文本编辑器内容的方法
    void display();

    // 功能函数
    // 获取整个文件的行数
    size_t getRows();

    // 获取当前行的长度
    rsize_t getCurrentLen(int y);

    // 获取当前行的gapbuffer对象
    GapBuffer getCurrentBuffer(int y);

    // 根据光标物理位置获取逻辑字符
    char getCurrentChar(int y, int x);
    char getNextChar(int y, int x);
    char getLastChar(int y, int x);

    // 获取前一个或者后一个光标
    int getLastCursor(int y, int x);
    int getNextCursor(int y, int x);

    // 获取解决制表符冲突后的坐标
    int handleTabCollision(int y, int x);

    // 判断是否是换行符
    bool isNewline(int y);
};


