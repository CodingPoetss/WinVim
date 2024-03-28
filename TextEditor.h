#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <fstream> // �����ļ�����
#include"GapBuffer.h"



class TextEditor {
private:
    std::vector<GapBuffer> lines; // ʹ��vector�洢ÿ�е�GapBuffer����

public:
    TextEditor();
    // �ļ�I/O����
    void loadFile(const std::string& filename);
    void saveFile(const std::string& filename);

    // ����չʾ�ı��༭�����ݵķ���
    void display();

    // ���ܺ���
    // ��ȡ�����ļ�������
    size_t getRows();

    // ��ȡ��ǰ�еĳ���
    rsize_t getCurrentLen(int y);

    // ��ȡ��ǰ�е�gapbuffer����
    GapBuffer getCurrentBuffer(int y);

    // ���ݹ������λ�û�ȡ�߼��ַ�
    char getCurrentChar(int y, int x);
    char getNextChar(int y, int x);
    char getLastChar(int y, int x);

    // ��ȡǰһ�����ߺ�һ�����
    int getLastCursor(int y, int x);
    int getNextCursor(int y, int x);

    // ��ȡ����Ʊ����ͻ�������
    int handleTabCollision(int y, int x);

    // �ж��Ƿ��ǻ��з�
    bool isNewline(int y);
};


