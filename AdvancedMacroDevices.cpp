#include"MainWindow.h"

int main(int argc, char** argv) {
    Window::App app = Window::InitApp();
    app.Loop();

    return 0;
}