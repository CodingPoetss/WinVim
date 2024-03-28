#pragma once
#include <iostream>
#include <vector>
#include<string>
#include<fstream>

using std::string;

#define TABWIDTH 8

// ȫ�ֹ��ߺ���

//// �ƶ����Ʊ�������һ��
//void toLastCursorInTab(int& realPos) {
//    int distanceToNextTabStop = TABWIDTH - ((realPos + 1) % TABWIDTH);
//    realPos += distanceToNextTabStop; // ������ƶ�����ǰ�����Ʊ�λ�����һ��
//}

/*
GapBuffer��һ�����л�������Ϊ�����ı��ӿڵ��ı�����
ÿ��������һ�����ȿɿ��Ƶ��ַ��������ṩһ��buffer��buffer�������ı�
������ڱ�����buffer����ڣ�vim�е�i��a��o����INSERTģʽ����ͨ��ָ��start������
�����ı��༭�Ķ����ԣ���ý�ÿ�е�������Ϊһ��Gapbuffer�������洢��Ҫ����ÿ�е������ԣ�ȷ��ÿ�ж�ֻ��һ��Gapbuffer����
�����ʹ��һ�����ݽṹ���洢���е�gapbuffer
���Ǽ��ػ򱣴��ı����ܡ������ļ�ʱ����Ҫ���ж�ȡ�ı����洢�����ݽṹ�У������ļ�ʱ���������ݽṹ������д���ļ���
*/
class GapBuffer {
private:
    std::vector<char> buffer; // �洢�ı���gap��vector
    size_t gapStart; // gap����ʼλ��
    size_t gapEnd;   // gap�Ľ���λ��

public:
    // ���캯������ʼ��buffer�Ĵ�С��gapλ��
    GapBuffer(size_t capacity = 1024);

    //���ߺ���
    void toLastCursorInTab(int& realPos);

    // ���������
    char& operator[](size_t index);
    const char& operator[](size_t index) const;

    // �����ַ�
    void insert(char c);
    void insert(string s);

    // ɾ���ַ�
    void backspace();  // ɾ��gapǰ��һ���ַ�
    void deleteChar(); // ɾ��gap���һ���ַ�

    // �����ƶ���꣨start��end������ǰ�ƶ�һλ������֮ǰstartǰ���ַ��ƶ���end֮��
    void moveCurrentGapLeft();
    void moveCurrentGapRight();

    // ��չgap�Ĵ�С
    // ��������⣬������ʹ��
    void expandGap();

    // �������
    void print();                              // ��ӡbuffer�����ݣ�����gap
    void printToFile(std::ofstream& output);   // ������gapbuffer�Ķ�����Ϊһ��������ı��ļ���ȥ

    // ����gapbuffer���е������ȣ������ɸ��ǵ��ܳ��ȣ�,�����Ʊ��
    size_t getLength();


    // ͨ����������λ�ü����gapbuffer�е�ʵ���߼��ַ�
    char getCurrentChar(int cursorX);
    char getNextChar(int cursorX);
    char getLastChar(int cursorX);

    // ��ȡ��һ������һ�����
    int getNextCursor(int cursorX);
    int getLastCursor(int cursorX);


    // ������ʱ���ܵĲ����Ʊ���м�ĳ�ͻ
    int handleTabCollision(int cursorX);


    ;
};


