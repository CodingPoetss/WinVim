#include"GapBuffer.h"

GapBuffer::GapBuffer(size_t capacity) : gapStart(0), gapEnd(capacity - 1) {
    buffer.resize(capacity);
}

void GapBuffer::toLastCursorInTab(int& realPos) {
    int distanceToNextTabStop = TABWIDTH - ((realPos + 1) % TABWIDTH);
    realPos += distanceToNextTabStop; // 将光标移动到当前所属制表位的最后一格
}

// 重载运算符，此版本允许修改元素
char& GapBuffer::operator[](size_t index) {
    // 如果索引在gap之前，直接返回该索引的元素
    if (index < gapStart) {
        return buffer[index];
    }
    // 如果索引在gap之后，将索引向前偏移gap的大小
    else {
        return buffer[index + gapEnd - gapStart + 1];
    }
}

// 为了保持const正确性，同时提供const版本的operator[]，此版本不允许修改
const char& GapBuffer::operator[](size_t index) const {
    // 如果索引在gap之前，直接返回该索引的元素
    if (index < gapStart) {
        return buffer[index];
    }
    // 如果索引在gap之后，将索引向前偏移gap的大小
    else {
        return buffer[index + gapEnd - gapStart + 1];
    }
}

// 在当前gapStart位置插入字符c
void GapBuffer::insert(char c) {
    if (gapStart > gapEnd) {
        // 如果gap已满，需要扩展buffer和gap大小
        expandGap();
    }
    buffer[gapStart++] = c;
}

void GapBuffer::insert(string s) {
    for (int i = 0; i < s.size(); i++)
        insert(char(s[i]));
}

// 删除gap前的一个字符
void GapBuffer::backspace() {
    if (gapStart > 0) {
        gapStart--;
    }
}

// 删除gap后的一个字符
void GapBuffer::deleteChar() {
    if (gapEnd < buffer.size() - 1) {
        gapEnd++;
    }
}

// 向左移动光标（start和end整体向前移动一位，并将之前start前的字符移动到end之后）
void GapBuffer::moveCurrentGapLeft() {
    if (gapStart > 0) {
        buffer[gapEnd--] = buffer[--gapStart];
    }
}

// 向右移动光标（start和end整体向后移动一位，并将之前end后的字符移动到start之前）
void GapBuffer::moveCurrentGapRight() {
    if (gapEnd < buffer.size() - 1) {
        buffer[gapStart++] = buffer[++gapEnd];
    }
}

// 扩展gap的大小
// 后续再理解，能正常使用
void GapBuffer::expandGap() {
    std::vector<char> newBuffer(buffer.size() * 2);
    std::copy(buffer.begin(), buffer.begin() + gapStart, newBuffer.begin());
    std::copy_backward(buffer.begin() + gapEnd + 1, buffer.end(), newBuffer.end());
    gapEnd = newBuffer.size() - (buffer.size() - gapEnd);
    buffer.swap(newBuffer);
}

// 打印buffer的内容，除了gap
void GapBuffer::print() {
    for (size_t i = 0; i < buffer.size(); i++) {
        if (i >= gapStart && i <= gapEnd) continue;
        std::cout << buffer[i];
    }
    std::cout << "\n";
}

// 将整个gapbuffer的对象作为一行输出到文本文件中去
void GapBuffer::printToFile(std::ofstream& output) {
    for (size_t i = 0; i < buffer.size(); i++) {
        if (i >= gapStart && i <= gapEnd) continue;
        output << buffer[i];
    }
    output << "\n";
}

// 计算gapbuffer单行的物理长度（即光标可覆盖的总长度）,考虑制表符
size_t GapBuffer::getLength() {
    int len = 0;
    for (size_t i = 0; i < buffer.size(); i++) {
        if (i >= gapStart && i <= gapEnd) continue;
        if (buffer[i] == '\t') {
            int distanceToNextTabStop = TABWIDTH - (len % TABWIDTH);
            len += distanceToNextTabStop; // 将光标移动到当前所属制表位的最后一格
        }
        else {
            len++;
        }
    }
    return len;
}

//// 获取当前光标位置（相对于行的起始）
//size_t getCursorPos() const {
//    return gapStart; // 假设光标始终位于gap的开始处
//}

//// 设置光标位置
//void setCursorPos(size_t newPos) {
//    // 实现移动光标到新位置的逻辑，可能需要调整gap的位置
//}


// 通过光标的物理位置计算出gapbuffer中的实际逻辑字符
char GapBuffer::getCurrentChar(int cursorX) {
    int realPos=0;

    // 先加后验,对于首个字符特殊处理
    for (size_t i = 0; i < buffer.size(); i++) {
        if (i >= gapStart && i <= gapEnd) continue;
        if (buffer[i] == '\t') toLastCursorInTab(realPos);
        else {
            if (i!=0) realPos++;
        }
        if (realPos == cursorX) return buffer[i];
    }
    return '\0';
}

char GapBuffer::getNextChar(int cursorX) {
    int realPos = 0;

    // 先加后验,对于首个字符特殊处理
    for (size_t i = 0; i < buffer.size(); i++) {
        if (i >= gapStart && i <= gapEnd) continue;
        if (buffer[i] == '\t') toLastCursorInTab(realPos);
        else {
            if (i != 0) realPos++;
        }
        if (realPos == cursorX) {
            for (size_t j = i+1; j < buffer.size(); j++) {
                if (j >= gapStart && j <= gapEnd) continue;
                return buffer[j];
            }
        }
    }
    return '\0';
}

char GapBuffer::getLastChar(int cursorX) {
    int realPos = 0;

    // 先加后验,对于首个字符特殊处理
    for (size_t i = 0; i < buffer.size(); i++) {
        if (i >= gapStart && i <= gapEnd) continue;
        if (buffer[i] == '\t') toLastCursorInTab(realPos);
        else {
            if (i != 0) realPos++;
        }
        if (realPos == cursorX && i > 0) {
            for (size_t j = i -1; j >0; j--) {
                if (j >= gapStart && j <= gapEnd) continue;
                return buffer[j];
            }
        }
    }
    return '\0';
}


// 获取上一个和下一个光标
int GapBuffer::getNextCursor(int cursorX) {
    int nextCursor = 0;
    int currentCursor = 0;

    // 先加后验,对于首个字符特殊处理
    for (size_t i = 0; i < buffer.size(); i++) {
        if (i >= gapStart && i <= gapEnd) continue;
        if (buffer[i] == '\t') toLastCursorInTab(currentCursor);
        else {
            if (i != 0) currentCursor++;
        }
        nextCursor = currentCursor;
        if (currentCursor == cursorX) {
            for (size_t j = i +1; j <buffer.size(); j++) {
                if (j >= gapStart && j <= gapEnd) continue;
                if (buffer[j] == '\t') toLastCursorInTab(nextCursor);
                else {
                    nextCursor++;
                }
                return nextCursor;
            }
        }
    }
    return currentCursor;
}

int GapBuffer::getLastCursor(int cursorX) {
    int lastCursor = -1;
    int currentCursor = 0;

    // 先加后验,对于首个字符特殊处理
    for (size_t i = 0; i < buffer.size(); i++) {
        if (i >= gapStart && i <= gapEnd) continue;
        if (buffer[i] == '\t') toLastCursorInTab(currentCursor);
        else {
            if (i != 0) currentCursor++;
        }
        if (currentCursor == cursorX) {
            return lastCursor;
        }
        lastCursor = currentCursor;
    }
    return -1;
}


// 处理换行时可能的插入制表符中间
int GapBuffer::handleTabCollision(int cursorX) {
    int currentCursor = 0;

    // 先加后验,对于首个字符特殊处理
    for (size_t i = 0; i < buffer.size(); i++) {
        if (i >= gapStart && i <= gapEnd) continue;
        if (buffer[i] == '\t') toLastCursorInTab(currentCursor);
        else {
            if (i != 0) currentCursor++;
        }
        if (currentCursor >=cursorX) {
            return currentCursor;
        }
    }
    return -1;
}


//size_t getLastCharIndex(int cursorX) {
//    int realPos = 0;

//    // 先加后验,对于首个字符特殊处理
//    for (size_t i = 0; i < buffer.size(); i++) {
//        if (i >= gapStart && i <= gapEnd) continue;
//        if (buffer[i] == '\t') toLastCursorInTab(realPos);
//        else {
//            if (i != 0) realPos++;
//        }
//        if (realPos == cursorX && i > 0) {
//            for (size_t j = i - 1; j > 0; j--) {
//                if (j >= gapStart && j <= gapEnd) continue;
//                return j;
//            }
//        }
//    }
//    return 0;
//}

//int getCursorByIndex(size_t index){
//    int cursorX = 0;

//    // 先加后验,对于首个字符特殊处理
//    for (size_t i = 0; i <= index; i++) {
//        if (i >= gapStart && i <= gapEnd) continue;
//        if (buffer[i] == '\t') toLastCursorInTab(cursorX);
//        else {
//            if (i != 0) cursorX++;
//        }
//    }
//    return cursorX;
//}

/*int getCursorPos(int bufferX) {
    int cursorPos = 0;
    for (size_t i = 0; i <= bufferX; i++) {
        if (i >= gapStart && i <= gapEnd) continue;
        if (buffer[i] == '\t')cursorPos += 4;
        else cursorPos++;
    }
    return cursorPos;
}*/


