#pragma once
#include"Window.h"
#include"Text.h"
#include"Character.h"
#include"List.h"
#include"Image.h"
#include"Grid.h"
#include"Button.h"
#include"Cpu.h"
#include"Decoder.h"
namespace Window
{
 
    static auto t1 = std::chrono::steady_clock::now();
    std::shared_ptr<AppStructure> UnitTest()
    {

        auto unitTest = AppWindow::Create("Unit Test", "Unit Test");
        auto column1 = ListItem::Create("page design col 1", 1, false);
        auto column2 = ListItem::Create("page design col 2", 1, true);
        auto column3 = ListItem::Create("page design col 3", 1, true);
        unitTest->AddNode(column1);        
        unitTest->AddNode(column2);
        unitTest->AddNode(column3);
        column1->AddNode(ButtonItem::Create("Bus Test 1", "Bus Test 1: linear propagation", 
            [column3]() mutable
            {

                column3->DeleteAllNodes();
                
                std::shared_ptr<Design::Cpu> testCpu=std::make_shared<Design::Cpu>(4, 4);
                testCpu->SetCell(0, 0,Design::ControlUnit::Create(1,1,1));
                testCpu->SetCell(1, 0,Design::Bus::Create(1,1,1));
                testCpu->SetCell(2, 0,Design::Bus::Create(1,1,1));
                testCpu->SetCell(2, 1,Design::Bus::Create(1,1,1));
                testCpu->SetCell(2, 2,Design::Bus::Create(1,1,1));
                testCpu->SetCell(2, 3, Design::Alu::Create(1,1,1));
                       
                       
                testCpu->Init();
                testCpu->GetCell<Design::Bus>(1, 0)->PrintPaths();
                testCpu->GetCell<Design::ControlUnit>(0, 0)->SetInput(Design::Data::Data(Design::DataType::MicroOpAlu,Design::ModuleType::ALU,-1,-1,Design::ModuleType::CONTROL_UNIT, testCpu->GetCell<Design::ControlUnit>(0, 0)->GetId()),0,0);

                auto gridView = testCpu->GetGridView();
                column3->AddNode(gridView);
                
  
                column3->OnPreRender([&,testCpu,column3]() mutable{   
                    if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - t1).count() > 500)
                    {
                        testCpu->Compute();

                        column3->DeleteAllNodes();
                        auto gridView = testCpu->GetGridView();
                        column3->AddNode(gridView);
                        t1 = std::chrono::steady_clock::now();
                    }
                });

            }, false, []() {return false; }));      


        column1->AddNode(ButtonItem::Create("Bus Test 2", "Bus Test 2: shortest path",
            [column3]() mutable
            {

                column3->DeleteAllNodes();

                std::shared_ptr<Design::Cpu> testCpu = std::make_shared<Design::Cpu>(7, 7);
                testCpu->SetCell(0, 0, Design::ControlUnit::Create(1,1,1));
                testCpu->SetCell(1, 0, Design::Bus::Create(1,1,1));
                testCpu->SetCell(2, 0, Design::Bus::Create(1,1,1));
                testCpu->SetCell(2, 1, Design::Bus::Create(1,1,1));
                testCpu->SetCell(2, 2, Design::Bus::Create(1,1,1));                
                testCpu->SetCell(0, 1, Design::Bus::Create(1,1,1));
                testCpu->SetCell(0, 2, Design::Bus::Create(1,1,1));
                testCpu->SetCell(0, 3, Design::Bus::Create(1,1,1));
                testCpu->SetCell(0, 4, Design::Bus::Create(1,1,1));
                testCpu->SetCell(1, 4, Design::Bus::Create(1,1,1));
                testCpu->SetCell(2, 4, Design::Bus::Create(1,1,1));
                testCpu->SetCell(2, 5, Design::Bus::Create(1,1,1));
                testCpu->SetCell(3, 5, Design::Bus::Create(1,1,1));
                testCpu->SetCell(4, 5, Design::Bus::Create(1,1,1));
                testCpu->SetCell(4, 6, Design::Bus::Create(1,1,1));
                testCpu->SetCell(5, 6, Design::Bus::Create(1,1,1));
                testCpu->SetCell(3, 2, Design::Bus::Create(1,1,1));
                testCpu->SetCell(4, 2, Design::Bus::Create(1,1,1));
                testCpu->SetCell(4, 1, Design::Bus::Create(1,1,1));
                testCpu->SetCell(5, 1, Design::Bus::Create(1,1,1));
                testCpu->SetCell(6, 1, Design::Bus::Create(1,1,1));
                testCpu->SetCell(6, 2, Design::Bus::Create(1,1,1));
                testCpu->SetCell(6, 3, Design::Bus::Create(1,1,1));
                testCpu->SetCell(6, 4, Design::Bus::Create(1,1,1));
                testCpu->SetCell(6, 5, Design::Bus::Create(1,1,1));

                testCpu->SetCell(6, 6, Design::Alu::Create(1,1,1));


                testCpu->Init();
                testCpu->GetCell<Design::Bus>(1, 0)->PrintPaths();
                testCpu->GetCell<Design::ControlUnit>(0, 0)->SetInput(Design::Data::Data(Design::DataType::MicroOpAlu, Design::ModuleType::ALU, -1, -1, Design::ModuleType::CONTROL_UNIT, testCpu->GetCell<Design::ControlUnit>(0, 0)->GetId()), 0,0);
                testCpu->GetCell<Design::ControlUnit>(0, 0)->SetNearestResourceScheduling();
                auto gridView = testCpu->GetGridView();
                column3->AddNode(gridView);


                column3->OnPreRender([&, testCpu, column3]() mutable {
                        if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - t1).count() > 500)
                        {
                            testCpu->Compute();
                            column3->DeleteAllNodes();
                            auto gridView = testCpu->GetGridView();
                            column3->AddNode(gridView);
                            t1 = std::chrono::steady_clock::now();
                        }
                    });

            }, false, []() {return false; }));




        column1->AddNode(ButtonItem::Create("Bus Test 3", "Bus Test 3: Round-Robin Routing",
            [column3]() mutable
            {

                column3->DeleteAllNodes();

                std::shared_ptr<Design::Cpu> testCpu = std::make_shared<Design::Cpu>(7, 7);
                testCpu->SetCell(0, 0, Design::ControlUnit::Create(1,1,1));
                testCpu->SetCell(1, 0, Design::Bus::Create(1,1,1));
                testCpu->SetCell(2, 0, Design::Bus::Create(1,1,1));
                testCpu->SetCell(2, 1, Design::Bus::Create(1,1,1));
                testCpu->SetCell(2, 2, Design::Bus::Create(1,1,1));
                testCpu->SetCell(0, 1, Design::Bus::Create(1,1,1));
                testCpu->SetCell(0, 2, Design::Bus::Create(1,1,1));
                testCpu->SetCell(0, 3, Design::Bus::Create(1,1,1));
                testCpu->SetCell(0, 4, Design::Bus::Create(1,1,1));
                testCpu->SetCell(1, 4, Design::Bus::Create(1,1,1));
                testCpu->SetCell(2, 4, Design::Bus::Create(1,1,1));
                testCpu->SetCell(2, 5, Design::Bus::Create(1,1,1));
                testCpu->SetCell(3, 5, Design::Bus::Create(1,1,1));
                testCpu->SetCell(4, 5, Design::Bus::Create(1,1,1));
                testCpu->SetCell(4, 6, Design::Bus::Create(1,1,1));
                testCpu->SetCell(5, 6, Design::Bus::Create(1,1,1));
                testCpu->SetCell(3, 2, Design::Bus::Create(1,1,1));
                testCpu->SetCell(4, 2, Design::Bus::Create(1,1,1));
                testCpu->SetCell(4, 1, Design::Bus::Create(1,1,1));
                testCpu->SetCell(5, 1, Design::Bus::Create(1,1,1));
                testCpu->SetCell(6, 1, Design::Bus::Create(1,1,1));
                testCpu->SetCell(6, 2, Design::Bus::Create(1,1,1));
                testCpu->SetCell(6, 3, Design::Bus::Create(1,1,1));
                testCpu->SetCell(6, 4, Design::Bus::Create(1,1,1));
                testCpu->SetCell(6, 5, Design::Bus::Create(1,1,1));
                testCpu->SetCell(6, 6, Design::Alu::Create(1,1,1));
                testCpu->Init();

                testCpu->GetCell<Design::Bus>(1, 0)->PrintPaths();
                testCpu->GetCell<Design::ControlUnit>(0, 0)->SetInput(Design::Data::Data(Design::DataType::MicroOpAlu, Design::ModuleType::ALU, -1, -1, Design::ModuleType::CONTROL_UNIT, testCpu->GetCell<Design::ControlUnit>(0, 0)->GetId()), 0,0);

                auto gridView = testCpu->GetGridView();
                column3->AddNode(gridView);


                column3->OnPreRender([&, testCpu, column3]() mutable {
                        testCpu->GetCell<Design::ControlUnit>(0, 0)->SetInput(Design::Data::Data(Design::DataType::MicroOpAlu, Design::ModuleType::ALU, -1, -1, Design::ModuleType::CONTROL_UNIT, testCpu->GetCell<Design::ControlUnit>(0, 0)->GetId()), 0,0);
                        if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - t1).count() > 500)
                        {
                            testCpu->Compute();

                            column3->DeleteAllNodes();
                            auto gridView = testCpu->GetGridView();
                            column3->AddNode(gridView);
                            t1 = std::chrono::steady_clock::now();
                        }
                    });

            }, false, []() {return false; }));


        column1->AddNode(ButtonItem::Create("Bus Test 4", "Bus Test 4: Bidirectional Transmission",
            [column3]() mutable
            {

                column3->DeleteAllNodes();

                std::shared_ptr<Design::Cpu> testCpu = std::make_shared<Design::Cpu>(5, 1);
                testCpu->SetCell(0, 0, Design::ControlUnit::Create(1,1,1));
                testCpu->SetCell(1, 0, Design::Bus::Create(1,1,1));
                testCpu->SetCell(2, 0, Design::Bus::Create(1,1,1));
                testCpu->SetCell(3, 0, Design::Bus::Create(1,1,1));                
                testCpu->SetCell(4, 0, Design::Alu::Create(1,1,1));

                testCpu->Init();

                testCpu->GetCell<Design::ControlUnit>(0, 0)->SetInput(Design::Data(Design::DataType::MicroOpAlu, Design::ModuleType::ALU, -1, -1, Design::ModuleType::CONTROL_UNIT, testCpu->GetCell<Design::ControlUnit>(0, 0)->GetId()), 0,0);

                auto gridView = testCpu->GetGridView();
                column3->AddNode(gridView);


                column3->OnPreRender([&, testCpu, column3]() mutable {                    
                        if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - t1).count() > 500)
                        {
                            testCpu->Compute();
                            column3->DeleteAllNodes();
                            auto gridView = testCpu->GetGridView();
                            column3->AddNode(gridView);
                            t1 = std::chrono::steady_clock::now();
                        }                    
                    });

            }, false, []() {return false; }));


        column1->AddNode(ButtonItem::Create("Bus Test 5", "Bus Test 5: Concurrent Bidirectional Transmission (will it deadlock?)",
            [column3]() mutable
            {

                column3->DeleteAllNodes();

                std::shared_ptr<Design::Cpu> testCpu = std::make_shared<Design::Cpu>(5, 1);
                testCpu->SetCell(0, 0, Design::ControlUnit::Create(1,1,1));
                testCpu->SetCell(1, 0, Design::Bus::Create(1,1,1));
                testCpu->SetCell(2, 0, Design::Bus::Create(1,1,1));
                testCpu->SetCell(3, 0, Design::Bus::Create(1,1,1));
                testCpu->SetCell(4, 0, Design::Alu::Create(1,1,1));

                testCpu->Init();

                auto gridView = testCpu->GetGridView();
                column3->AddNode(gridView);


                column3->OnPreRender([&, testCpu, column3]() mutable {
                    static int countCmd = 0;                
                    if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - t1).count() > 500)
                    {
                        countCmd++;
                        if (countCmd % 2 == 0)
                            testCpu->GetCell<Design::ControlUnit>(0, 0)->SetInput(Design::Data(Design::DataType::MicroOpAlu, Design::ModuleType::ALU, -1, -1, Design::ModuleType::CONTROL_UNIT, testCpu->GetCell<Design::ControlUnit>(0, 0)->GetId()), 0,0);
                        testCpu->Compute();
                        column3->DeleteAllNodes();
                        column3->AddNode(TextItem::Create("Num completed op.", [testCpu]() { return std::string("Completed operations=") + std::to_string(testCpu->GetCell<Design::ControlUnit>(0, 0)->GetCompletedOperationCount()); }, 2, false));
                        auto gridView = testCpu->GetGridView();
                        column3->AddNode(gridView);
                        t1 = std::chrono::steady_clock::now();
                    }


                    });

            }, false, []() {return false; }));





        column1->AddNode(ButtonItem::Create("Bus Test 6", "Bus Test 6: Three-way Communication",
            [column3]() mutable
            {
                column3->DeleteAllNodes();
                std::shared_ptr<Design::Cpu> testCpu = std::make_shared<Design::Cpu>(6, 3);
                testCpu->SetCell(0, 0, Design::ControlUnit::Create(1,1,1));
                testCpu->SetCell(1, 0, Design::Bus::Create(1,1,1));
                testCpu->SetCell(2, 0, Design::Bus::Create(1,1,1));
                testCpu->SetCell(3, 0, Design::Bus::Create(1,1,1));
                testCpu->SetCell(4, 0, Design::Bus::Create(1,1,1));
                testCpu->SetCell(5, 0, Design::Alu::Create(1,1,1));
                testCpu->SetCell(2, 1, Design::Bus::Create(1,1,1));
                testCpu->SetCell(2, 2, Design::Decoder::Create(1,1,1));
                testCpu->GetCell<Design::ControlUnit>(0, 0)->SetInput(Design::Data(Design::DataType::MicroOpDecode, Design::ModuleType::DECODER, -1, -1, Design::ModuleType::CONTROL_UNIT, testCpu->GetCell<Design::ControlUnit>(0, 0)->GetId()), 0,0);
                testCpu->Init();

                auto gridView = testCpu->GetGridView();
                column3->AddNode(gridView);

                column3->OnPreRender([&, testCpu, column3]() mutable {
                        static int countCmd = 0;
                        if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - t1).count() > 500)
                        {
                            testCpu->Compute();
                            column3->DeleteAllNodes();
                            column3->AddNode(TextItem::Create("Num completed op.", [testCpu]() { return std::string("Completed operations=") + std::to_string(testCpu->GetCell<Design::ControlUnit>(0, 0)->GetCompletedOperationCount()); }, 2, false));
                            auto gridView = testCpu->GetGridView();
                            column3->AddNode(gridView);
                            t1 = std::chrono::steady_clock::now();
                        }
                    });

            }, false, []() {return false; }));

        column1->AddNode(ButtonItem::Create("Bus Test 7", "Bus Test 7: Three-way Single-Channel-Bus, Low Bandwidth Scenario",
            [column3]() mutable
            {
                column3->DeleteAllNodes();
                std::shared_ptr<Design::Cpu> testCpu = std::make_shared<Design::Cpu>(6, 3);
                testCpu->SetCell(0, 0, Design::ControlUnit::Create(1, 1, 1));
                testCpu->SetCell(1, 0, Design::Bus::Create(1, 1, 1));
                testCpu->SetCell(2, 0, Design::Bus::Create(1, 1, 1));
                testCpu->SetCell(3, 0, Design::Bus::Create(1, 1, 1));
                testCpu->SetCell(4, 0, Design::Bus::Create(1, 1, 1));
                testCpu->SetCell(5, 0, Design::Alu::Create(1, 1, 1));
                testCpu->SetCell(2, 1, Design::Bus::Create(1, 1, 1));
                testCpu->SetCell(2, 2, Design::Decoder::Create(1, 1, 1));
                
                testCpu->Init();

                auto gridView = testCpu->GetGridView();
                column3->AddNode(gridView);

                column3->OnPreRender([&, testCpu, column3]() mutable {
                    static int countCmd = 0;
                    if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - t1).count() > 500)
                    {
                        if(countCmd++%3 == 0)
                            testCpu->GetCell<Design::ControlUnit>(0, 0)->SetInput(Design::Data(Design::DataType::MicroOpDecode, Design::ModuleType::DECODER, -1, -1, Design::ModuleType::CONTROL_UNIT, testCpu->GetCell<Design::ControlUnit>(0, 0)->GetId()), 0, 0);
                        testCpu->Compute();
                        column3->DeleteAllNodes();
                        column3->AddNode(TextItem::Create("Num completed op.", [testCpu]() { return std::string("Completed operations=") + std::to_string(testCpu->GetCell<Design::ControlUnit>(0, 0)->GetCompletedOperationCount()); }, 2, false));
                        auto gridView = testCpu->GetGridView();
                        column3->AddNode(gridView);
                        t1 = std::chrono::steady_clock::now();
                    }
                    });

            }, false, []() {return false; }));



        column1->AddNode(ButtonItem::Create("Bus Test 8", "Bus Test 8: Three-way Single-Channel-Bus, Medium Bandwidth Scenario",
            [column3]() mutable
            {
                column3->DeleteAllNodes();
                std::shared_ptr<Design::Cpu> testCpu = std::make_shared<Design::Cpu>(6, 3);
                testCpu->SetCell(0, 0, Design::ControlUnit::Create(1, 1, 1));
                testCpu->SetCell(1, 0, Design::Bus::Create(1, 1, 1));
                testCpu->SetCell(2, 0, Design::Bus::Create(1, 1, 1));
                testCpu->SetCell(3, 0, Design::Bus::Create(1, 1, 1));
                testCpu->SetCell(4, 0, Design::Bus::Create(1, 1, 1));
                testCpu->SetCell(5, 0, Design::Alu::Create(1, 1, 1));
                testCpu->SetCell(2, 1, Design::Bus::Create(1, 1, 1));
                testCpu->SetCell(2, 2, Design::Decoder::Create(1, 1, 1));

                testCpu->Init();

                auto gridView = testCpu->GetGridView();
                column3->AddNode(gridView);

                column3->OnPreRender([&, testCpu, column3]() mutable {
                    static int countCmd = 0;
                    if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - t1).count() > 500)
                    {
                        if (countCmd++ % 2 == 0)
                            testCpu->GetCell<Design::ControlUnit>(0, 0)->SetInput(Design::Data(Design::DataType::MicroOpDecode, Design::ModuleType::DECODER, -1, -1, Design::ModuleType::CONTROL_UNIT, testCpu->GetCell<Design::ControlUnit>(0, 0)->GetId()), 0, 0);
                        testCpu->Compute();
                        column3->DeleteAllNodes();
                        column3->AddNode(TextItem::Create("Num completed op.", [testCpu]() { return std::string("Completed operations=") + std::to_string(testCpu->GetCell<Design::ControlUnit>(0, 0)->GetCompletedOperationCount()); }, 2, false));
                        auto gridView = testCpu->GetGridView();
                        column3->AddNode(gridView);
                        t1 = std::chrono::steady_clock::now();
                    }
                    });

            }, false, []() {return false; }));


        column1->AddNode(ButtonItem::Create("Bus Test 9", "Bus Test 9: Three-way Single-Channel-Bus, Max Bandwidth Scenario",
            [column3]() mutable
            {
                column3->DeleteAllNodes();
                std::shared_ptr<Design::Cpu> testCpu = std::make_shared<Design::Cpu>(6, 3);
                testCpu->SetCell(0, 0, Design::ControlUnit::Create(1, 1, 1));
                testCpu->SetCell(1, 0, Design::Bus::Create(1, 1, 1));
                testCpu->SetCell(2, 0, Design::Bus::Create(1, 1, 1));
                testCpu->SetCell(3, 0, Design::Bus::Create(1, 1, 1));
                testCpu->SetCell(4, 0, Design::Bus::Create(1, 1, 1));
                testCpu->SetCell(5, 0, Design::Alu::Create(1, 1, 1));
                testCpu->SetCell(2, 1, Design::Bus::Create(1, 1, 1));
                testCpu->SetCell(2, 2, Design::Decoder::Create(1, 1, 1));

                testCpu->Init();

                auto gridView = testCpu->GetGridView();
                column3->AddNode(gridView);

                column3->OnPreRender([&, testCpu, column3]() mutable {
                    static int countCmd = 0;
                    if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - t1).count() > 500)
                    {
                        testCpu->GetCell<Design::ControlUnit>(0, 0)->SetInput(Design::Data(Design::DataType::MicroOpDecode, Design::ModuleType::DECODER, -1, -1, Design::ModuleType::CONTROL_UNIT, testCpu->GetCell<Design::ControlUnit>(0, 0)->GetId()), 0, 0);
                        testCpu->Compute();
                        column3->DeleteAllNodes();
                        column3->AddNode(TextItem::Create("Num completed op.", [testCpu]() { return std::string("Completed operations=") + std::to_string(testCpu->GetCell<Design::ControlUnit>(0, 0)->GetCompletedOperationCount()); }, 2, false));
                        auto gridView = testCpu->GetGridView();
                        column3->AddNode(gridView);
                        t1 = std::chrono::steady_clock::now();
                    }
                    });

            }, false, []() {return false; }));


        column1->AddNode(ButtonItem::Create("Bus Test 10", "Bus Test 10: Three-way Dual-Channel-Bus, Medium Bandwidth Scenario",
            [column3]() mutable
            {
                column3->DeleteAllNodes();
                std::shared_ptr<Design::Cpu> testCpu = std::make_shared<Design::Cpu>(6, 3);
                testCpu->SetCell(0, 0, Design::ControlUnit::Create(1, 1, 1));
                testCpu->SetCell(1, 0, Design::Bus::Create(1, 1, 2));
                testCpu->SetCell(2, 0, Design::Bus::Create(1, 1, 2));
                testCpu->SetCell(3, 0, Design::Bus::Create(1, 1, 2));
                testCpu->SetCell(4, 0, Design::Bus::Create(1, 1, 2));
                testCpu->SetCell(5, 0, Design::Alu::Create(1, 1, 1));
                testCpu->SetCell(2, 1, Design::Bus::Create(1, 1, 2));
                testCpu->SetCell(2, 2, Design::Decoder::Create(1, 1, 1));

                testCpu->Init();

                auto gridView = testCpu->GetGridView();
                column3->AddNode(gridView);

                column3->OnPreRender([&, testCpu, column3]() mutable {
                    static int countCmd = 0;
                    if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - t1).count() > 500)
                    {
                        if (countCmd++ % 2 == 0)
                        testCpu->GetCell<Design::ControlUnit>(0, 0)->SetInput(Design::Data(Design::DataType::MicroOpDecode, Design::ModuleType::DECODER, -1, -1, Design::ModuleType::CONTROL_UNIT, testCpu->GetCell<Design::ControlUnit>(0, 0)->GetId()), 0, 0);
                        testCpu->Compute();
                        column3->DeleteAllNodes();
                        column3->AddNode(TextItem::Create("Num completed op.", [testCpu]() { return std::string("Completed operations=") + std::to_string(testCpu->GetCell<Design::ControlUnit>(0, 0)->GetCompletedOperationCount()); }, 2, false));
                        auto gridView = testCpu->GetGridView();
                        column3->AddNode(gridView);
                        t1 = std::chrono::steady_clock::now();
                    }
                    });

            }, false, []() {return false; }));


        column1->AddNode(ButtonItem::Create("Bus Test 11", "Bus Test 11: Three-way Dual-Channel-Bus, Max Bandwidth Scenario",
            [column3]() mutable
            {
                column3->DeleteAllNodes();
                std::shared_ptr<Design::Cpu> testCpu = std::make_shared<Design::Cpu>(6, 3);
                testCpu->SetCell(0, 0, Design::ControlUnit::Create(1, 1, 1));
                testCpu->SetCell(1, 0, Design::Bus::Create(1, 1, 2));
                testCpu->SetCell(2, 0, Design::Bus::Create(1, 1, 2));
                testCpu->SetCell(3, 0, Design::Bus::Create(1, 1, 2));
                testCpu->SetCell(4, 0, Design::Bus::Create(1, 1, 2));
                testCpu->SetCell(5, 0, Design::Alu::Create(1, 1, 1));
                testCpu->SetCell(2, 1, Design::Bus::Create(1, 1, 2));
                testCpu->SetCell(2, 2, Design::Decoder::Create(1, 1, 1));

                testCpu->Init();

                auto gridView = testCpu->GetGridView();
                column3->AddNode(gridView);

                column3->OnPreRender([&, testCpu, column3]() mutable {
                    static int countCmd = 0;
                    if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - t1).count() > 500)
                    {

                        testCpu->GetCell<Design::ControlUnit>(0, 0)->SetInput(Design::Data(Design::DataType::MicroOpDecode, Design::ModuleType::DECODER, -1, -1, Design::ModuleType::CONTROL_UNIT, testCpu->GetCell<Design::ControlUnit>(0, 0)->GetId()), 0, 0);
                        testCpu->Compute();
                        column3->DeleteAllNodes();
                        column3->AddNode(TextItem::Create("Num completed op.", [testCpu]() { return std::string("Completed operations=") + std::to_string(testCpu->GetCell<Design::ControlUnit>(0, 0)->GetCompletedOperationCount()); }, 2, false));
                        auto gridView = testCpu->GetGridView();
                        column3->AddNode(gridView);
                        t1 = std::chrono::steady_clock::now();
                    }
                    });

            }, false, []() {return false; }));


        column1->AddNode(ButtonItem::Create("Bus Test 12", "Bus Test 12: Three-way Triple-Channel-Bus, Max Bandwidth Scenario",
            [column3]() mutable
            {
                column3->DeleteAllNodes();
                std::shared_ptr<Design::Cpu> testCpu = std::make_shared<Design::Cpu>(6, 3);
                testCpu->SetCell(0, 0, Design::ControlUnit::Create(1, 1, 1));
                testCpu->SetCell(1, 0, Design::Bus::Create(1, 1, 3));
                testCpu->SetCell(2, 0, Design::Bus::Create(1, 1, 3));
                testCpu->SetCell(3, 0, Design::Bus::Create(1, 1, 3));
                testCpu->SetCell(4, 0, Design::Bus::Create(1, 1, 3));
                testCpu->SetCell(5, 0, Design::Alu::Create(1, 1, 1));
                testCpu->SetCell(2, 1, Design::Bus::Create(1, 1, 3));
                testCpu->SetCell(2, 2, Design::Decoder::Create(1, 1, 1));

                testCpu->Init();

                auto gridView = testCpu->GetGridView();
                column3->AddNode(gridView);

                column3->OnPreRender([&, testCpu, column3]() mutable {
                    static int countCmd = 0;
                    if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - t1).count() > 500)
                    {

                        testCpu->GetCell<Design::ControlUnit>(0, 0)->SetInput(Design::Data(Design::DataType::MicroOpDecode, Design::ModuleType::DECODER, -1, -1, Design::ModuleType::CONTROL_UNIT, testCpu->GetCell<Design::ControlUnit>(0, 0)->GetId()), 0, 0);
                        testCpu->Compute();
                        column3->DeleteAllNodes();
                        column3->AddNode(TextItem::Create("Num completed op.", [testCpu]() { return std::string("Completed operations=") + std::to_string(testCpu->GetCell<Design::ControlUnit>(0, 0)->GetCompletedOperationCount()); }, 2, false));
                        auto gridView = testCpu->GetGridView();
                        column3->AddNode(gridView);
                        t1 = std::chrono::steady_clock::now();
                    }
                    });

            }, false, []() {return false; }));


        column1->AddNode(ButtonItem::Create("Bus Test 13", "Bus Test 13: Three-way Triple-Channel-Bus, Medium Bandwidth Scenario",
            [column3]() mutable
            {
                column3->DeleteAllNodes();
                std::shared_ptr<Design::Cpu> testCpu = std::make_shared<Design::Cpu>(6, 3);
                testCpu->SetCell(0, 0, Design::ControlUnit::Create(1, 1, 1));
                testCpu->SetCell(1, 0, Design::Bus::Create(1, 1, 3));
                testCpu->SetCell(2, 0, Design::Bus::Create(1, 1, 3));
                testCpu->SetCell(3, 0, Design::Bus::Create(1, 1, 3));
                testCpu->SetCell(4, 0, Design::Bus::Create(1, 1, 3));
                testCpu->SetCell(5, 0, Design::Alu::Create(1, 1, 1));
                testCpu->SetCell(2, 1, Design::Bus::Create(1, 1, 3));
                testCpu->SetCell(2, 2, Design::Decoder::Create(1, 1, 1));

                testCpu->Init();

                auto gridView = testCpu->GetGridView();
                column3->AddNode(gridView);

                column3->OnPreRender([&, testCpu, column3]() mutable {
                    static int countCmd = 0;
                    if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - t1).count() > 500)
                    {
                        if(countCmd++%2 == 0)
                            testCpu->GetCell<Design::ControlUnit>(0, 0)->SetInput(Design::Data(Design::DataType::MicroOpDecode, Design::ModuleType::DECODER, -1, -1, Design::ModuleType::CONTROL_UNIT, testCpu->GetCell<Design::ControlUnit>(0, 0)->GetId()), 0, 0);
                        testCpu->Compute();
                        column3->DeleteAllNodes();
                        column3->AddNode(TextItem::Create("Num completed op.", [testCpu]() { return std::string("Completed operations=") + std::to_string(testCpu->GetCell<Design::ControlUnit>(0, 0)->GetCompletedOperationCount()); }, 2, false));
                        auto gridView = testCpu->GetGridView();
                        column3->AddNode(gridView);
                        t1 = std::chrono::steady_clock::now();
                    }
                    });

            }, false, []() {return false; }));


        column1->AddNode(ButtonItem::Create("Bus Test 14", "Bus Test 14: Three-way Quad-Channel-Bus, Medium Bandwidth Scenario",
            [column3]() mutable
            {
                column3->DeleteAllNodes();
                std::shared_ptr<Design::Cpu> testCpu = std::make_shared<Design::Cpu>(6, 3);
                testCpu->SetCell(0, 0, Design::ControlUnit::Create(1, 1, 1));
                testCpu->SetCell(1, 0, Design::Bus::Create(1, 1, 4));
                testCpu->SetCell(2, 0, Design::Bus::Create(1, 1, 4));
                testCpu->SetCell(3, 0, Design::Bus::Create(1, 1, 4));
                testCpu->SetCell(4, 0, Design::Bus::Create(1, 1, 4));
                testCpu->SetCell(5, 0, Design::Alu::Create(1, 1, 1));
                testCpu->SetCell(2, 1, Design::Bus::Create(1, 1, 4));
                testCpu->SetCell(2, 2, Design::Decoder::Create(1, 1, 1));

                testCpu->Init();

                auto gridView = testCpu->GetGridView();
                column3->AddNode(gridView);

                column3->OnPreRender([&, testCpu, column3]() mutable {
                    static int countCmd = 0;
                    if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - t1).count() > 500)
                    {
                        if (countCmd++ % 2 == 0)
                            testCpu->GetCell<Design::ControlUnit>(0, 0)->SetInput(Design::Data(Design::DataType::MicroOpDecode, Design::ModuleType::DECODER, -1, -1, Design::ModuleType::CONTROL_UNIT, testCpu->GetCell<Design::ControlUnit>(0, 0)->GetId()), 0, 0);
                        testCpu->Compute();
                        column3->DeleteAllNodes();
                        column3->AddNode(TextItem::Create("Num completed op.", [testCpu]() { return std::string("Completed operations=") + std::to_string(testCpu->GetCell<Design::ControlUnit>(0, 0)->GetCompletedOperationCount()); }, 2, false));
                        auto gridView = testCpu->GetGridView();
                        column3->AddNode(gridView);
                        t1 = std::chrono::steady_clock::now();
                    }
                    });

            }, false, []() {return false; }));


        column1->AddNode(ButtonItem::Create("Bus Test 15", "Bus Test 15: Three-way Penta-Channel-Bus, Medium Bandwidth Scenario",
            [column3]() mutable
            {
                column3->DeleteAllNodes();
                std::shared_ptr<Design::Cpu> testCpu = std::make_shared<Design::Cpu>(6, 3);
                testCpu->SetCell(0, 0, Design::ControlUnit::Create(1, 1, 1));
                testCpu->SetCell(1, 0, Design::Bus::Create(1, 1, 5));
                testCpu->SetCell(2, 0, Design::Bus::Create(1, 1, 5));
                testCpu->SetCell(3, 0, Design::Bus::Create(1, 1, 5));
                testCpu->SetCell(4, 0, Design::Bus::Create(1, 1, 5));
                testCpu->SetCell(5, 0, Design::Alu::Create(1, 1, 1));
                testCpu->SetCell(2, 1, Design::Bus::Create(1, 1, 5));
                testCpu->SetCell(2, 2, Design::Decoder::Create(1, 1, 1));

                testCpu->Init();

                auto gridView = testCpu->GetGridView();
                column3->AddNode(gridView);

                column3->OnPreRender([&, testCpu, column3]() mutable {
                    static int countCmd = 0;
                    if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - t1).count() > 500)
                    {
                        if (countCmd++ % 2 == 0)
                            testCpu->GetCell<Design::ControlUnit>(0, 0)->SetInput(Design::Data(Design::DataType::MicroOpDecode, Design::ModuleType::DECODER, -1, -1, Design::ModuleType::CONTROL_UNIT, testCpu->GetCell<Design::ControlUnit>(0, 0)->GetId()), 0, 0);
                        testCpu->Compute();
                        column3->DeleteAllNodes();
                        column3->AddNode(TextItem::Create("Num completed op.", [testCpu]() { return std::string("Completed operations=") + std::to_string(testCpu->GetCell<Design::ControlUnit>(0, 0)->GetCompletedOperationCount()); }, 2, false));
                        auto gridView = testCpu->GetGridView();
                        column3->AddNode(gridView);
                        t1 = std::chrono::steady_clock::now();
                    }
                    });

            }, false, []() {return false; }));
        unitTest->Hide();
        return unitTest;
    }
}
