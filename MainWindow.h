#pragma once
#include <memory>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>

#include"State.h"
#include"Window.h"
#include"Text.h"
#include"Button.h"
namespace Window
{
    struct AppFields
    {
        AppFields()
        {
            if (!glfwInit())
                exit(1);

            window = glfwCreateWindow(1280, 720, "Advanced Macro Devices", NULL, NULL);
            if (!window)
            {
                glfwTerminate();
                exit(1);
            }
            glfwMakeContextCurrent(window);

            // Initialize ImGui
            IMGUI_CHECKVERSION();
            ImGui::CreateContext();
            io = &ImGui::GetIO();

            ImGui::StyleColorsDark();
            ImGui_ImplGlfw_InitForOpenGL(window, true);
            ImGui_ImplOpenGL3_Init("#version 130");
        }

        ~AppFields()
        {
            // Cleanup
            ImGui_ImplOpenGL3_Shutdown();
            ImGui_ImplGlfw_Shutdown();
            ImGui::DestroyContext();

            glfwDestroyWindow(window);
            glfwTerminate();
        }

        GLFWwindow* window;
        ImGuiIO* io;
        AppStructure structure;
    };

    class App
    {
    public:
        App()
        {
            _fields = std::make_shared<AppFields>();
        }

        void AddComponent(std::shared_ptr<AppStructure> component)
        {
            _fields->structure.AddNode(component);
        }


        void Loop()
        {
            // Main loop
            while (!glfwWindowShouldClose(_fields->window)) {
                glfwPollEvents();

                ImGui_ImplOpenGL3_NewFrame();
                ImGui_ImplGlfw_NewFrame();
                ImGui::NewFrame();

                // Advanced Macro Devices
                _fields->structure.Compute();
                _fields->structure.Render();


                ImGui::Render();
                int display_w, display_h;
                glfwGetFramebufferSize(_fields->window, &display_w, &display_h);
                glViewport(0, 0, display_w, display_h);
                glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
                glClear(GL_COLOR_BUFFER_BIT);
                ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

                glfwSwapBuffers(_fields->window);
            }
        }
    private:
        std::shared_ptr<AppFields> _fields;
    };

    App InitApp()
    {
        App app;
        auto window = AppWindow::Create(States::GetVersion());
        window->AddNode(TextItem::Create("text item", "Welcome to Advanced Macro Devices",3));
        app.AddComponent(window);

        auto createCharacter = AppWindow::Create("Character Creation");
        createCharacter->AddNode(TextItem::Create("Stats", "Stats",3));
        createCharacter->AddNode(TextItem::Create("Learning", "Learning",2));
        createCharacter->Hide();
        app.AddComponent(createCharacter);

        auto menu = AppWindow::Create("Menu");
        menu->AddNode(ButtonItem::Create("New Game", "New Game", []() {}));
        menu->AddNode(ButtonItem::Create("Create Character", "Create Character", [createCharacter]() { createCharacter->Show(); }));
        menu->AddNode(ButtonItem::Create("Load Game", "Load Game", []() {}));
        menu->AddNode(ButtonItem::Create("Design CPU", "Design CPU", []() {}));
        menu->AddNode(ButtonItem::Create("Exit", "Exit", []() {}));
        app.AddComponent(menu);



        return app;
    }
}