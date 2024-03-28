#pragma once
#include <iostream>
#include <string>
#include <Windows.h> // ���ڿ���̨����
#include <conio.h> // ����_getch()
#include <sstream> // ���� std::ostringstream
#include <iomanip> // ���� std::setw �� std::setfill
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
    ע�⣬cursor������ǰ��չ������������ı��ľ���λ������Ƶ�
    ������Ҫ��������������������ʾ����ڵ�ǰ��λ��
    */
    int cursorX = 0; // ����Xλ��
    int cursorY = 0; // ����Yλ��
    int visibleStart; // ��ǰ���ӵ��ı����о���λ����ʼ����
    int visibleEnd;  // ��ǰ���ӵ��ı����о���λ�ý�������
    Mode mode;
    std::string filename;

public:
    InteractiveEditor();
    InteractiveEditor(const std::string& filename);

    // ����
    void clearScreen();
    void clearScreenExceptStatusBar();

    // �ƶ���굽ָ��λ��
    void moveCursor(int x, int y);

    // ����״̬�����ı�
    void updateStatusBar();
    void drawText();

    // �ı�����
    bool scrollIfNeeded();

    // ���㹤��
    int calculatePercent();
    int getConsoleY();

    // �����������߼�
    void handleKeyboardInput();

    // ����ƶ��߼�
    void moveCursorUp();
    void moveCursorDown();
    void moveCursorLeft();
    void moveCursorRight();

    // �����༭���߼�
    void run();
};


