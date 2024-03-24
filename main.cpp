#include"InteractiveEditor.cpp"

int main(int argc, char* argv[]) {
    /*if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>\n";
        return 1;
    }

    ConsoleEditor consoleEditor(argv[1]);*/
  /*  ConsoleEditor consoleEditor("test.txt");
    consoleEditor.run();*/

   /* GapBuffer gp;
    gp.insert("hello\thello");
    gp.print();
    std::cout << gp.getCurrentChar(12) << std::endl;
    std::cout << gp.getNextChar(12) << std::endl;
    std::cout << static_cast<int>(gp.getNextChar(66)) << std::endl;*/

    InteractiveEditor interactiveEditor("test.txt");
    interactiveEditor.run();

  /*  TextEditor textEditor;
    textEditor.loadFile("text.txt");
    textEditor.display();*/

    return 0;

}
