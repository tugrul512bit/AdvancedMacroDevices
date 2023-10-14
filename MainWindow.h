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
#include"UnitTest.h"
#include"CpuDesign.h"
#include<mutex>
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
            window = glfwCreateWindow(1600, 900, "Advanced Macro Devices: Deadlock Tycoon", NULL, NULL);
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

        void Close()
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
        std::mutex mut;
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
            bool run = true;
            while (run && !glfwWindowShouldClose(_fields->window)) {
             
              
                glfwPollEvents(); 

                ImGui_ImplOpenGL3_NewFrame(); 
                ImGui_ImplGlfw_NewFrame();
                ImGui::NewFrame();

                // Advanced Macro Devices
                _fields->structure.Calculate();
                _fields->structure.Render();


                ImGui::Render();
                int display_w, display_h;
                glfwGetFramebufferSize(_fields->window, &display_w, &display_h);
                glViewport(0, 0, display_w, display_h);
                glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
                glClear(GL_COLOR_BUFFER_BIT);
                ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

                glfwSwapBuffers(_fields->window);
                run = _fields->running;
                
            }

            std::cout << "app closing..." << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(2));
            _fields->Close();
        }



        Character* GetCurrentCharacter()
        {
            return &_fields->currentCharacter;
        }

        void CreateNewCharacter()
        {
            _fields->currentCharacter = Character();
        }

        ~App() {  }
    private:
        std::shared_ptr<AppFields> _fields;
    };

    App InitApp()
    {
        App app;
        auto window = AppWindow::Create("version window",States::GetVersion());
        window->AddNode(TextItem::Create("text item", []() {return "Welcome to Advanced Macro Devices: Deadlock Tycoon"; }, 3, false));
        app.AddComponent(window);

        auto createCharacter = CreateCharacter(app.GetCurrentCharacter());
        app.AddComponent(createCharacter);

        auto cpuDesign = CpuDesign(app.GetCurrentCharacter());
        app.AddComponent(cpuDesign);


        auto unitTest = UnitTest();
        app.AddComponent(unitTest);

        auto menu = AppWindow::Create("Menu","Menu");
        menu->AddNode(ButtonItem::Create("New Game", "New Game",                    []() {},false, []() {return false; }));
        menu->AddNode(ButtonItem::Create("Create Character", "Create Character", [createCharacter, app]() mutable { app.CreateNewCharacter();  createCharacter->Show(); }, false, []() {return false; }));
        menu->AddNode(ButtonItem::Create("Load Game", "Load Game",                  []() {},false, []() {return false; }));
        menu->AddNode(ButtonItem::Create("Design CPU", "Design CPU", [cpuDesign]() { cpuDesign->Show(); }, false, []() {return false; }));
        auto unitTests = ButtonItem::Create("Unit Test Main", "Unit Test", [unitTest]() { unitTest->Show(); }, false, []() {return false; });
        unitTests->AsPtr<ButtonItem>()->SetDefaultColor(ImVec4(0, 0.3, 0, 1));
        menu->AddNode(unitTests);
        menu->AddNode(ButtonItem::Create("Exit", "Exit", [app]() mutable { app.Stop(); }, false, []() {return false; }));
        app.AddComponent(menu);




        return app;
    }
}