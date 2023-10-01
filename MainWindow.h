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
#include"List.h"
#include"Image.h"
#include"CreateCharacter.h"
namespace Window
{
    struct AppFields
    {
        AppFields()
        {
            running = true;

            if (!glfwInit())
                exit(1);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); 
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
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
            ImGui_ImplOpenGL3_Init("#version 330");
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
        Character currentCharacter;
        bool running;
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

        void Stop()
        {
            _fields->running = false;
        }

        void Loop()
        {
            // Main loop
            while (!glfwWindowShouldClose(_fields->window)) {
                if (!_fields->running)
                    break;
                glfwPollEvents();

                ImGui_ImplOpenGL3_NewFrame();
                ImGui_ImplGlfw_NewFrame();
                ImGui::NewFrame();

                // Advanced Macro Devices
                _fields->structure.Calculate();
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

        Character* GetCurrentCharacter()
        {
            return &_fields->currentCharacter;
        }

        void CreateNewCharacter()
        {
            _fields->currentCharacter = Character();
        }
    private:
        std::shared_ptr<AppFields> _fields;
    };

    App InitApp()
    {
        App app;
        auto window = AppWindow::Create("version window",States::GetVersion());
        window->AddNode(TextItem::Create("text item", []() {return "Welcome to Advanced Macro Devices"; }, 3, false));
        app.AddComponent(window);

        auto createCharacter = CreateCharacter(app.GetCurrentCharacter());
        app.AddComponent(createCharacter);



        auto cpuDesign = AppWindow::Create("CPU Design","CPU Design");
        auto techList = ListItem::Create("Tech List", 3, false);
        auto aluItem = ListItem::Create("ALU item", 2, false);
        aluItem->AddNode(TextItem::Create("ALU Tech Name", []() { return "ALU"; }, 3, false));
        aluItem->AddNode(ImageItem::Create("ALU Image", true, Images::AluImage()));

        auto cacheItem = ListItem::Create("Cache item", 2, false);
        cacheItem->AddNode(TextItem::Create("Cache Tech Name", []() { return "Cache"; }, 3, false));
        cacheItem->AddNode(ImageItem::Create("Cache Image", true, Images::CacheImage()));

        auto registerItem = ListItem::Create("Register item", 2, false);
        registerItem->AddNode(TextItem::Create("Register Tech Name", []() { return "Register"; }, 3, false));
        registerItem->AddNode(ImageItem::Create("Register Image", true, Images::RegisterImage()));

        auto busItem = ListItem::Create("Bus item", 2, false);
        busItem->AddNode(TextItem::Create("Bus Tech Name", []() { return "Bus"; }, 3, false));
        busItem->AddNode(ImageItem::Create("Bus Image", true, Images::BusImage()));


        techList->AddNode(aluItem);
        techList->AddNode(cacheItem);
        techList->AddNode(registerItem);
        techList->AddNode(busItem);

        cpuDesign->AddNode(techList);

        cpuDesign->Hide();
        app.AddComponent(cpuDesign);

        auto menu = AppWindow::Create("Menu","Menu");
        menu->AddNode(ButtonItem::Create("New Game", "New Game",                    []() {},false, []() {return false; }));
        menu->AddNode(ButtonItem::Create("Create Character", "Create Character", [createCharacter, app]() mutable { app.CreateNewCharacter();  createCharacter->Show(); }, false, []() {return false; }));
        menu->AddNode(ButtonItem::Create("Load Game", "Load Game",                  []() {},false, []() {return false; }));
        menu->AddNode(ButtonItem::Create("Design CPU", "Design CPU", [cpuDesign]() { cpuDesign->Show(); }, false, []() {return false; }));
        menu->AddNode(ButtonItem::Create("Exit", "Exit", [app]() mutable { app.Stop(); }, false, []() {return false; }));
        app.AddComponent(menu);


     
        return app;
    }
}