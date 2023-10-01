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

        auto decoderItem = ListItem::Create("Decoder item", 2, false);
        decoderItem->AddNode(TextItem::Create("Decoder Tech Name", []() { return "Decoder"; }, 3, false));
        decoderItem->AddNode(ImageItem::Create("Decoder Image", true, Images::EmptyGridCellImage()));

        auto fpuItem = ListItem::Create("FPU item", 2, false);
        fpuItem->AddNode(TextItem::Create("FPU Tech Name", []() { return "FPU"; }, 3, false));
        fpuItem->AddNode(ImageItem::Create("FPU Image", true, Images::EmptyGridCellImage()));

        techList->AddNode(aluItem);
        techList->AddNode(cacheItem);
        techList->AddNode(registerItem);
        techList->AddNode(busItem);
        techList->AddNode(decoderItem);
        techList->AddNode(fpuItem);

        auto cpuGrid = GridItem<8, 8>::Create("CpuGrid", 1.0, true);

        leftRightMid->AddNode(techList);
        leftRightMid->AddNode(TextItem::Create("New Column", []() { return "   "; }, 4, true));
        leftRightMid->AddNode(cpuGrid);

        leftRightMid2->AddNode(TextItem::Create("Requirements For Unlocking", []() { return "Requirements For Unlocking"; }, 3, false));
        leftRightMid2->AddNode(TextItem::Create("Empty Column", []() { return "     "; }, 3, true));
        leftRightMid2->AddNode(TextItem::Create("Requirements For Production", []() { return "Requirements For Production"; }, 3, true));

        cpuDesign->AddNode(leftRightMid);
        cpuDesign->AddNode(leftRightMid2);
        cpuDesign->Hide();
        return cpuDesign;
    }
}
