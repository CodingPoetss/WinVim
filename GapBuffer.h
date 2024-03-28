#pragma once
#include <iostream>
#include <vector>
#include<string>
#include<fstream>

using std::string;

#define TABWIDTH 8

// 全局工具函数

//// 移动到制表符的最后一格
//void toLastCursorInTab(int& realPos) {
//    int distanceToNextTabStop = TABWIDTH - ((realPos + 1) % TABWIDTH);
//    realPos += distanceToNextTabStop; // 将光标移动到当前所属制表位的最后一格
//}

/*
GapBuffer是一个带有缓冲区作为插入文本接口的文本对象
每个对象是一个长度可控制的字符向量，提供一块buffer，buffer两侧是文本
光标所在被称作buffer的入口，vim中的i和a、o进入INSERT模式可以通过指定start来进行
由于文本编辑的独立性，最好将每行的内容作为一个Gapbuffer对象来存储（要考虑每行的伸缩性，确保每行都只是一个Gapbuffer对象）
最后再使用一个数据结构来存储所有的gapbuffer
考虑加载或保存文本功能。加载文件时，需要按行读取文本并存储在数据结构中；保存文件时，遍历数据结构并按行写入文件。
*/
class GapBuffer {
private:
    std::vector<char> buffer; // 存储文本和gap的vector
    size_t gapStart; // gap的起始位置
    size_t gapEnd;   // gap的结束位置

public:
    // 构造函数，初始化buffer的大小和gap位置
    GapBuffer(size_t capacity = 1024);

    //工具函数
    void toLastCursorInTab(int& realPos);

    // 重载运算符
    char& operator[](size_t index);
    const char& operator[](size_t index) const;

    // 插入字符
    void insert(char c);
    void insert(string s);

    // 删除字符
    void backspace();  // 删除gap前的一个字符
    void deleteChar(); // 删除gap后的一个字符

    // 左右移动光标（start和end整体向前移动一位，并将之前start前的字符移动到end之后）
    void moveCurrentGapLeft();
    void moveCurrentGapRight();

    // 扩展gap的大小
    // 后续再理解，能正常使用
    void expandGap();

    // 输出函数
    void print();                              // 打印buffer的内容，除了gap
    void printToFile(std::ofstream& output);   // 将整个gapbuffer的对象作为一行输出到文本文件中去

    // 计算gapbuffer单行的物理长度（即光标可覆盖的总长度）,考虑制表符
    size_t getLength();


    // 通过光标的物理位置计算出gapbuffer中的实际逻辑字符
    char getCurrentChar(int cursorX);
    char getNextChar(int cursorX);
    char getLastChar(int cursorX);

    // 获取上一个和下一个光标
    int getNextCursor(int cursorX);
    int getLastCursor(int cursorX);


    // 处理换行时可能的插入制表符中间的冲突
    int handleTabCollision(int cursorX);


    ;
};


