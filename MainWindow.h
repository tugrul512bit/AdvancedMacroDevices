#pragma once
#include <memory>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>

#include"State.h"
#include"Window.h"
#include"Text.h"
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

                // Your ImGui code here
                _fields->structure.Render();
                //ImGui::Begin("Hello, world!");
                //ImGui::Text("This is a simple ImGui application.");
                //ImGui::End();

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
        window->AddNode(TextItem::Create("text item", "Welcome to Advanced Macro Devices"));
        app.AddComponent(window);
        
        return app;
    }
}