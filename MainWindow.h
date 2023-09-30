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
namespace Window
{
    struct AppFields
    {
        AppFields()
        {
            // initialize player data
            freeStatPoints = 10;
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
        int freeStatPoints;
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
        int GetFreeStatPoints()
        {
            return _fields->freeStatPoints;
        }
        void IncrementFreeStatPoints(int n=1)
        {
            _fields->freeStatPoints+=n;
        }
        void DecrementFreeStatPoints(int n=1)
        {
            _fields->freeStatPoints-=n;
        }
        Character* GetCurrentCharacter()
        {
            return &_fields->currentCharacter;
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

        auto createCharacter = AppWindow::Create("Character Creation","Character Generator");
        createCharacter->AddNode(TextItem::Create("Stats", []() {return "Stats"; },3,false));

        createCharacter->AddNode(TextItem::Create("Learning", []() {return "Learning     "; },2, false));
        createCharacter->AddNode(ButtonItem::Create("Learning Decrement", "-", [app]() mutable { app.GetCurrentCharacter()->DecrementLearning(); app.IncrementFreeStatPoints(std::max(app.GetCurrentCharacter()->GetLearningModifier(), 1)); }, true, []() {return false; }));
        createCharacter->AddNode(TextItem::Create("Learning Value",                 [app]() mutable { return std::to_string(app.GetCurrentCharacter()->GetLearning()); },2,true));
        createCharacter->AddNode(ButtonItem::Create("Learning Increment", "+",      [app]() mutable { if (app.GetFreeStatPoints() > 0) { app.DecrementFreeStatPoints(std::max(app.GetCurrentCharacter()->GetLearningModifier(), 1)); app.GetCurrentCharacter()->IncrementLearning(); } }, true, []() {return false; }));

        createCharacter->AddNode(TextItem::Create("Rnd", []() {return "R&D          "; }, 2, false));
        createCharacter->AddNode(ButtonItem::Create("Rnd Decrement", "-",           [app]() mutable { app.GetCurrentCharacter()->DecrementRnd(); app.IncrementFreeStatPoints(std::max(app.GetCurrentCharacter()->GetRndModifier(), 1)); }, true, []() {return false; }));
        createCharacter->AddNode(TextItem::Create("Rnd Value",                      [app]() mutable { return std::to_string(app.GetCurrentCharacter()->GetRnd()); }, 2, true));
        createCharacter->AddNode(ButtonItem::Create("Rnd Increment", "+",           [app]() mutable { if (app.GetFreeStatPoints() > 0) {  app.DecrementFreeStatPoints(std::max(app.GetCurrentCharacter()->GetRndModifier(), 1)); app.GetCurrentCharacter()->IncrementRnd(); } }, true, []() {return false; }));

        createCharacter->AddNode(TextItem::Create("Strength", []() {return "Strength     "; }, 2, false));
        createCharacter->AddNode(ButtonItem::Create("Strength Decrement", "-",      [app]() mutable { app.GetCurrentCharacter()->DecrementStrength(); app.IncrementFreeStatPoints(std::max(app.GetCurrentCharacter()->GetStrengthModifier(), 1)); }, true, []() {return false; }));
        createCharacter->AddNode(TextItem::Create("Strength Value",                 [app]() mutable { return std::to_string(app.GetCurrentCharacter()->GetStrength()); }, 2, true));
        createCharacter->AddNode(ButtonItem::Create("Strength Increment", "+",      [app]() mutable { if (app.GetFreeStatPoints() > 0) {  app.DecrementFreeStatPoints(std::max(app.GetCurrentCharacter()->GetStrengthModifier(), 1)); app.GetCurrentCharacter()->IncrementStrength(); } }, true, []() {return false; }));

        createCharacter->AddNode(TextItem::Create("Health", []() {return "Health       "; }, 2, false));
        createCharacter->AddNode(ButtonItem::Create("Health Decrement", "-",        [app]() mutable { app.GetCurrentCharacter()->DecrementHealth(); app.IncrementFreeStatPoints(std::max(app.GetCurrentCharacter()->GetHealthModifier(), 1)); }, true, []() {return false; }));
        createCharacter->AddNode(TextItem::Create("Health Value",                   [app]() mutable { return std::to_string(app.GetCurrentCharacter()->GetHealth()); }, 2, true));
        createCharacter->AddNode(ButtonItem::Create("Health Increment", "+",        [app]() mutable { if (app.GetFreeStatPoints() > 0) {  app.DecrementFreeStatPoints(std::max(app.GetCurrentCharacter()->GetHealthModifier(), 1)); app.GetCurrentCharacter()->IncrementHealth(); } }, true, []() {return false; }));

        createCharacter->AddNode(TextItem::Create("Intellligence", []() {return "Intelligence "; }, 2, false));
        createCharacter->AddNode(ButtonItem::Create("Intelligence Decrement", "-",  [app]() mutable { app.GetCurrentCharacter()->DecrementIntelligence(); app.IncrementFreeStatPoints(std::max(app.GetCurrentCharacter()->GetIntelligenceModifier(), 1)); }, true, []() {return false; }));
        createCharacter->AddNode(TextItem::Create("Intelligence Value",             [app]() mutable { return std::to_string(app.GetCurrentCharacter()->GetIntelligence()); }, 2, true));
        createCharacter->AddNode(ButtonItem::Create("Intelligence Increment", "+",  [app]() mutable { if (app.GetFreeStatPoints() > 0) {  app.DecrementFreeStatPoints(std::max(app.GetCurrentCharacter()->GetIntelligenceModifier(), 1)); app.GetCurrentCharacter()->IncrementIntelligence(); } }, true, []() {return false; }));

        createCharacter->AddNode(TextItem::Create("Charisma", []() {return "Charisma     "; }, 2, false));
        createCharacter->AddNode(ButtonItem::Create("Charisma Decrement", "-",      [app]() mutable { app.GetCurrentCharacter()->DecrementCharisma(); app.IncrementFreeStatPoints(std::max(app.GetCurrentCharacter()->GetCharismaModifier(), 1)); }, true, []() {return false; }));
        createCharacter->AddNode(TextItem::Create("Charisma Value",                 [app]() mutable { return std::to_string(app.GetCurrentCharacter()->GetCharisma()); }, 2, true));
        createCharacter->AddNode(ButtonItem::Create("Charisma Increment", "+",      [app]() mutable { if (app.GetFreeStatPoints() > 0) {  app.DecrementFreeStatPoints(std::max(app.GetCurrentCharacter()->GetCharismaModifier(), 1)); app.GetCurrentCharacter()->IncrementCharisma(); } }, true, []() {return false; }));

        createCharacter->AddNode(TextItem::Create("Leadership", []() {return "Leadership   "; }, 2, false));
        createCharacter->AddNode(ButtonItem::Create("Leadership Decrement", "-",    [app]() mutable { app.GetCurrentCharacter()->DecrementLeadership(); app.IncrementFreeStatPoints(std::max(app.GetCurrentCharacter()->GetLeadershipModifier(), 1)); }, true, []() {return false; }));
        createCharacter->AddNode(TextItem::Create("Leadership Value",               [app]() mutable { return std::to_string(app.GetCurrentCharacter()->GetLeadership()); }, 2, true));
        createCharacter->AddNode(ButtonItem::Create("Leadership Increment", "+",    [app]() mutable { if (app.GetFreeStatPoints() > 0) {  app.DecrementFreeStatPoints(std::max(app.GetCurrentCharacter()->GetLeadershipModifier(), 1)); app.GetCurrentCharacter()->IncrementLeadership(); } }, true, []() {return false; }));

        createCharacter->AddNode(TextItem::Create("Free Points", []() {return "Free Stat Points     "; }, 2, false));
        
        createCharacter->AddNode(TextItem::Create("Free Points Value", [app]() mutable { return std::to_string(app.GetFreeStatPoints()); }, 2, true));
        

        createCharacter->AddNode(TextItem::Create("New Line", []() {return " "; }, 3, false));
        createCharacter->AddNode(TextItem::Create("Inventory", []() {return "Inventory"; }, 3, false));
        createCharacter->AddNode(TextItem::Create("Coins", []() {return "Coins        "; }, 2, false));
        createCharacter->AddNode(TextItem::Create("Coins Value", [app]() mutable 
            { 
                int extra = app.GetCurrentCharacter()->GetCoinsEquivalentOfMissingStats(app.GetFreeStatPoints());
                return std::to_string(app.GetCurrentCharacter()->GetCoins() + extra);
            }, 2, true));

        createCharacter->AddNode(TextItem::Create("New Line", []() {return " "; }, 3, false));
        createCharacter->AddNode(ButtonItem::Create("Create Character Button", "Create", [app]() mutable {}, false, [app]()mutable { return app.GetFreeStatPoints() < 0; }));
        createCharacter->Hide();
        app.AddComponent(createCharacter);



        auto cpuDesign = AppWindow::Create("CPU Design","CPU Design");

        auto techList = ListItem::Create("Tech List", 3, false);
        auto aluItem = ListItem::Create("ALU item", 2, false);
        aluItem->AddNode(TextItem::Create("ALU Tech Name", []() { return "ALU"; }, 3, false));
        aluItem->AddNode(ImageItem::Create("ALU Image", true, 64, 64));
        techList->AddNode(aluItem);

        cpuDesign->AddNode(techList);

        cpuDesign->Hide();
        app.AddComponent(cpuDesign);

        auto menu = AppWindow::Create("Menu","Menu");
        menu->AddNode(ButtonItem::Create("New Game", "New Game",                    []() {},false, []() {return false; }));
        menu->AddNode(ButtonItem::Create("Create Character", "Create Character",    [createCharacter]() { createCharacter->Show(); },false, []() {return false; }));
        menu->AddNode(ButtonItem::Create("Load Game", "Load Game",                  []() {},false, []() {return false; }));
        menu->AddNode(ButtonItem::Create("Design CPU", "Design CPU", [cpuDesign]() { cpuDesign->Show(); }, false, []() {return false; }));
        menu->AddNode(ButtonItem::Create("Exit", "Exit", [app]() mutable { app.Stop(); }, false, []() {return false; }));
        app.AddComponent(menu);


     
        return app;
    }
}