#pragma once
#include"Window.h"
#include"Text.h"
#include"Character.h"
#include"List.h"
#include"Image.h"
namespace Window
{
    std::shared_ptr<AppStructure> CpuDesign(Character* character)
    {
        auto cpuDesign = AppWindow::Create("CPU Design", "CPU Design");
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
        return cpuDesign;
    }
}
