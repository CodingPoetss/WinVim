#include"TextEditor.h"

TextEditor::TextEditor(){}
// �����ļ�
void TextEditor::loadFile(const std::string& filename) {
    std::ifstream input(filename);
    if (input.fail()) {
        std::cerr << "�޷����ļ�: " << filename << std::endl;
        return;
    }
    std::string line;
    while (std::getline(input, line)) {
        GapBuffer gb(line.size() + 1024); // Ϊÿ��Ԥ������Ŀռ�
        gb.insert(line);
        lines.push_back(gb);
    }
    input.close();
}

// �����ļ�
// Ŀǰ��֧�ָ�дģʽ
void TextEditor::saveFile(const std::string& filename) {
    std::ofstream output(filename);
    if (output.fail()) {
        std::cerr << "�޷����ļ�: " << filename << std::endl;
        return;
    }
    for (auto& gb : lines) {
        gb.printToFile(output); // ����GapBuffer��һ����ӡ���ļ��ķ���
    }
    output.close();
    std::cout << "Have been saved in : " << filename << std::endl;
}

// ����չʾ�ı��༭�����ݵķ���
void TextEditor::display() {
    for (auto& gb : lines) {
        gb.print(); // ʹ��GapBuffer��print������ӡÿһ��
    }
}

// ��ȡ�����ļ�������
size_t TextEditor::getRows() {
    return lines.size();
}

// ��ȡ��ǰ�еĳ���
rsize_t TextEditor::getCurrentLen(int y) {
    return lines[y].getLength();
}

// ��ȡ��ǰ�е�gapbuffer����
GapBuffer TextEditor::getCurrentBuffer(int y) {
    return lines[y];
}

// ��ȡ��ǰ�е�ĳ���߼��ַ�
char TextEditor::getCurrentChar(int y, int x) {
    return lines[y].getCurrentChar(x);
}

// ��ȡ��ǰ����һ���߼��ַ�
char TextEditor::getNextChar(int y, int x) {
    return lines[y].getNextChar(x);
}

// ��ȡ��ǰ����һ���߼��ַ�
char TextEditor::getLastChar(int y, int x) {
    return lines[y].getLastChar(x);
}

// ��ȡ��һ�����
int TextEditor::getLastCursor(int y, int x) {
    return lines[y].getLastCursor(x);
}

// ��ȡ��һ�����
int TextEditor::getNextCursor(int y, int x) {
    return lines[y].getNextCursor(x);
}

// ��ȡ����Ʊ����ͻ�������
int TextEditor::handleTabCollision(int y, int x) {
    return lines[y].handleTabCollision(x);
}

// �ж��Ƿ��ǻ��з�
bool TextEditor::isNewline(int y) {
    return lines[y].getLength() == 0;
}
