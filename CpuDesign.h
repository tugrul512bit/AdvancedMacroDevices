#pragma once
#include"Window.h"
#include"Text.h"
#include"Character.h"
#include"List.h"
#include"Image.h"
#include"Grid.h"
namespace Window
{
    std::shared_ptr<AppStructure> CpuDesign(Character* character)
    {
        auto cpuDesign = AppWindow::Create("CPU Design", "CPU Design");
        cpuDesign->AddNode(TextItem::Create("Drag drop hint", []() { return "Drag & drop modules to the grid to build your CPU."; }, 3, false));
        cpuDesign->AddNode(TextItem::Create("New Line", []() { return " "; }, 4, false));

        auto leftRightMid = ListItem::Create("Left and right of Cpu Design", 3, false);
        auto leftRightMid2 = ListItem::Create("Left and right of Cpu Design 2", 3, false);
        auto leftRightMid3 = ListItem::Create("Left and right of Cpu Design 3", 3, false);
        auto leftRightMid4 = ListItem::Create("Left and right of Cpu Design 4", 3, false);

        auto techList = ListItem::Create("Tech List", 3, false);
        
        auto aluItem = ListItem::Create("ALU item", 2, false);
        aluItem->AddNode(TextItem::Create("ALU Tech Name", []() { return "ALU"; }, 3, false));
        aluItem->AddNode(ImageItem::Create("ALU Image", true, Images::AluImage(),0,false,1));

        auto cacheBankItem = ListItem::Create("Cache Bank item", 2, false);
        cacheBankItem->AddNode(TextItem::Create("Cache Bank Tech Name", []() { return "Cache Bank"; }, 3, false));
        cacheBankItem->AddNode(ImageItem::Create("Cache Bank Image", true, Images::CacheBankImage(),0,false,1));

        auto cacheControlItem = ListItem::Create("Cache Control item", 2, false);
        cacheControlItem->AddNode(TextItem::Create("Cache Control Tech Name", []() { return "Cache Control"; }, 3, false));
        cacheControlItem->AddNode(ImageItem::Create("Cache Control Image", true, Images::CacheControlImage(), 0, false, 1));


        auto registerItem = ListItem::Create("Register item", 2, false);
        registerItem->AddNode(TextItem::Create("Register Tech Name", []() { return "Register"; }, 3, false));
        registerItem->AddNode(ImageItem::Create("Register Image", true, Images::RegisterImage(),0,false,1));

        auto busItem = ListItem::Create("Bus item", 2, false);
        busItem->AddNode(TextItem::Create("Bus Tech Name", []() { return "Bus"; }, 3, false));
        busItem->AddNode(ImageItem::Create("Bus Image", true, Images::BusImage(),0,false,1));

        auto decoderItem = ListItem::Create("Decoder item", 2, false);
        decoderItem->AddNode(TextItem::Create("Decoder Tech Name", []() { return "Decoder"; }, 3, false));
        decoderItem->AddNode(ImageItem::Create("Decoder Image", true, Images::DecoderImage(), 0, false, 1));

        auto fpuItem = ListItem::Create("FPU item", 2, false);
        fpuItem->AddNode(TextItem::Create("FPU Tech Name", []() { return "FPU"; }, 3, false));
        fpuItem->AddNode(ImageItem::Create("FPU Image", true, Images::FpuImage(),0,false,1));

        auto controlUnitItem = ListItem::Create("Control Unit item", 2, false);
        controlUnitItem->AddNode(TextItem::Create("Control Unit Tech Name", []() { return "Control Unit"; }, 3, false));
        controlUnitItem->AddNode(ImageItem::Create("Control Unit Image", true, Images::ControlUnitImage(), 0, false, 1));

        techList->AddNode(aluItem);
        techList->AddNode(cacheBankItem);
        techList->AddNode(cacheControlItem);
        techList->AddNode(registerItem);
        techList->AddNode(busItem);
        techList->AddNode(decoderItem);
        techList->AddNode(fpuItem);
        techList->AddNode(controlUnitItem);

        auto cpuGrid = GridItem::Create("CpuGrid",8,8, 1.0, true);

        leftRightMid->AddNode(techList);
        leftRightMid->AddNode(TextItem::Create("New Column", []() { return "   "; }, 4, true));
        leftRightMid->AddNode(cpuGrid);

        leftRightMid2->AddNode(TextItem::Create("Requirements For Unlocking", []() { return "Requirements For Unlocking"; }, 3, false));
        leftRightMid2->AddNode(TextItem::Create("Empty Column", []() { return "     "; }, 3, true));
        leftRightMid2->AddNode(TextItem::Create("Requirements For Production", []() { return "Requirements For Production"; }, 3, true));

        leftRightMid3->AddNode(TextItem::Create("Transistors", []() { return "Transistors"; }, 3, false));
        leftRightMid3->AddNode(TextItem::Create("Empty Column", []() { return "     "; }, 3, true));
        leftRightMid3->AddNode(TextItem::Create("Thermal Dissipation Power", []() { return "Thermal Dissipation Power"; }, 3, true));

        auto testAlgorithm = TextItem::Create("Test Algorithm", []() { return "Test Algorithm"; }, 3, false);
        testAlgorithm->AddHoverPopup(TextItem::Create("Test Algorithm Name", []() { return "Sinebench Mona Lisa Render (by Maxxon)"; }, 2, false));
        leftRightMid4->AddNode(testAlgorithm);
        leftRightMid4->AddNode(TextItem::Create("Empty Column", []() { return "     "; }, 3, true));
        leftRightMid4->AddNode(TextItem::Create("Production Failure Probability", []() { return "Production Failure Probability"; }, 3, true));


        cpuDesign->AddNode(leftRightMid);
        cpuDesign->AddNode(leftRightMid2);
        cpuDesign->AddNode(leftRightMid3);
        cpuDesign->AddNode(leftRightMid4);
        cpuDesign->Hide();
        return cpuDesign;
    }
}
