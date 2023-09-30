#pragma once
#include <memory>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>
struct AppFields
{
    AppFields()
    {
        // Initialize GLFW
        if (!glfwInit())
            exit(1);

        window = glfwCreateWindow(1280, 720, "ImGui Example", NULL, NULL);
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
};
class App
{
public:
    App()
    {
        fields = std::make_shared<AppFields>();
    }

    void Loop()
    {
        // Main loop
        while (!glfwWindowShouldClose(fields->window)) {
            glfwPollEvents();

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            // Your ImGui code here
            ImGui::Begin("Hello, world!");
            ImGui::Text("This is a simple ImGui application.");
            ImGui::End();

            ImGui::Render();
            int display_w, display_h;
            glfwGetFramebufferSize(fields->window, &display_w, &display_h);
            glViewport(0, 0, display_w, display_h);
            glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
            glClear(GL_COLOR_BUFFER_BIT);
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(fields->window);
        }
    }
private:
    std::shared_ptr<AppFields> fields;
};