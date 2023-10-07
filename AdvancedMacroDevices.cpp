#include"MainWindow.h"

int main(int argc, char** argv) {
    {
        Window::App app = Window::InitApp();
        app.Loop();
    }
    std::cout << "app closed" << std::endl;
    return 0;
}