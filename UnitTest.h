#pragma once
#include"Window.h"
#include"Text.h"
#include"Character.h"
#include"List.h"
#include"Image.h"
#include"Grid.h"
#include"Button.h"
#include"Cpu.h"
namespace Window
{

    std::shared_ptr<AppStructure> UnitTest()
    {
        static Design::Cpu testCpu(4, 4);
        auto unitTest = AppWindow::Create("Unit Test", "Unit Test");
        auto column1 = ListItem::Create("page design col 1", 1, false);
        auto column2 = ListItem::Create("page design col 2", 1, true);
        auto column3 = ListItem::Create("page design col 3", 1, true);
        unitTest->AddNode(column1);        
        unitTest->AddNode(column2);
        unitTest->AddNode(column3);
        column1->AddNode(ButtonItem::Create("Bus Test 1", "Bus Test 1: linear propagation", 
            [column3]()
            {
                column3->DeleteAllNodes();
                testCpu = Design::Cpu(4, 4);
                testCpu.SetCell<Design::ControlUnit>(0, 0, 1,1000);
                testCpu.SetCell<Design::Bus>(1, 0, 1, 1000);
                testCpu.SetCell<Design::Bus>(2, 0, 1, 1000);
                testCpu.SetCell<Design::Bus>(2, 1, 1, 1000);
                testCpu.SetCell<Design::Bus>(2, 2, 1, 1000);
                testCpu.SetCell<Design::Alu>(2, 3, 1, 1000);
                auto gridView = testCpu.GetGridView();
                column3->AddNode(gridView);
            }, true, []() {return false; }));        
        unitTest->Hide();
        return unitTest;
    }
}
