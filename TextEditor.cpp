#include <iostream>
#include <vector>
#include <string>
#include <fstream> // 用于文件操作
#include"GapBuffer.cpp"
// 保留之前定义的 GapBuffer 类 ...


class TextEditor {
private:
    std::vector<GapBuffer> lines; // 使用vector存储每行的GapBuffer对象

public:
    // 载入文件
    void loadFile(const std::string& filename) {
        std::ifstream input(filename);
        if (input.fail()) {
            std::cerr << "无法打开文件: " << filename << std::endl;
            return;
        }
        std::string line;
        while (std::getline(input, line)) {
            GapBuffer gb(line.size() + 1024); // 为每行预留额外的空间
            gb.insert(line);
            lines.push_back(gb);
        }
        input.close();
    }

    // 保存文件
    // 目前仅支持覆写模式
    void saveFile(const std::string& filename) {
        std::ofstream output(filename);
        if (output.fail()) {
            std::cerr << "无法打开文件: " << filename << std::endl;
            return;
        }
        for (auto& gb : lines) {
            gb.printToFile(output); // 假设GapBuffer有一个打印到文件的方法
        }
        output.close();
        std::cout << "Have been saved in : " << filename << std::endl;
    }

    // 用于展示文本编辑器内容的方法
    void display() {
        for (auto& gb : lines) {
            gb.print(); // 使用GapBuffer的print方法打印每一行
        }
    }

    // 获取整个文件的行数
    size_t getRows() {
        return lines.size();
    }

    // 获取当前行的长度
    rsize_t getCurrentLen(int y) {
        return lines[y].getLength();
    }

    // 获取当前行的gapbuffer对象
    GapBuffer getCurrentBuffer(int y) {
        return lines[y];
    }

    // 获取当前行的某个逻辑字符
    char getCurrentChar(int y, int x) {
        return lines[y].getCurrentChar(x);
    }

    // 获取当前行下一个逻辑字符
    char getNextChar(int y, int x) {
        return lines[y].getNextChar(x);
    }

    // 获取当前行上一个逻辑字符
    char getLastChar(int y, int x) {
        return lines[y].getLastChar(x);
    }

    // 获取上一个光标
    int getLastCursor(int y, int x) {
        return lines[y].getLastCursor(x);
    }

    // 获取下一个光标
    int getNextCursor(int y, int x) {
        return lines[y].getNextCursor(x);
    }

    // 获取解决制表符冲突后的坐标
    int handleTabCollision(int y, int x) {
        return lines[y].handleTabCollision(x);
    }

    // 判断是否是换行符
    bool isNewline(int y) {
        return lines[y].getLength() == 0 ;
    }
};


