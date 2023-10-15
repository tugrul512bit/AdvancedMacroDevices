#pragma once
#include"AppStructure.h"
#include"Window.h"
#include"Text.h"
#include"Button.h"
#include"DropdownList.h"
#include"Character.h"
namespace Window
{
	std::shared_ptr<AppStructure> CreateCharacter(Character * character)
	{

        auto createCharacter = AppWindow::Create("Character Creation", "Character Generator");


        createCharacter->AddNode(TextItem::Create("Player Class", []() {return "Player Class"; }, 3, false));
        createCharacter->AddNode(DropdownListItem::Create("Class Selection", "Class Selection", []() {}, false, []() {return false; }));

        createCharacter->AddNode(TextItem::Create("Stats", []() {return "Stats"; }, 3, false));

        createCharacter->AddNode(TextItem::Create("Learning", []() {return "Learning     "; }, 2, false));
        createCharacter->AddNode(ButtonItem::Create("Learning Decrement", "-", [character]() mutable {character->DecrementLearning(); character->IncrementFreeStatPoints(std::max(character->GetLearningModifier(), 1)); }, true, []() {return false; }));
        createCharacter->AddNode(TextItem::Create("Learning Value", [character]() mutable { return std::to_string(character->GetLearning()); }, 2, true));
        createCharacter->AddNode(ButtonItem::Create("Learning Increment", "+", [character]() mutable { if (character->GetFreeStatPoints() > 0) { character->DecrementFreeStatPoints(std::max(character->GetLearningModifier(), 1)); character->IncrementLearning(); } }, true, []() {return false; }));

        createCharacter->AddNode(TextItem::Create("Rnd", []() {return "R&D          "; }, 2, false));
        createCharacter->AddNode(ButtonItem::Create("Rnd Decrement", "-", [character]() mutable { character->DecrementRnd(); character->IncrementFreeStatPoints(std::max(character->GetRndModifier(), 1)); }, true, []() {return false; }));
        createCharacter->AddNode(TextItem::Create("Rnd Value", [character]() mutable { return std::to_string(character->GetRnd()); }, 2, true));
        createCharacter->AddNode(ButtonItem::Create("Rnd Increment", "+", [character]() mutable { if (character->GetFreeStatPoints() > 0) { character->DecrementFreeStatPoints(std::max(character->GetRndModifier(), 1)); character->IncrementRnd(); } }, true, []() {return false; }));

        createCharacter->AddNode(TextItem::Create("Strength", []() {return "Strength     "; }, 2, false));
        createCharacter->AddNode(ButtonItem::Create("Strength Decrement", "-", [character]() mutable { character->DecrementStrength(); character->IncrementFreeStatPoints(std::max(character->GetStrengthModifier(), 1)); }, true, []() {return false; }));
        createCharacter->AddNode(TextItem::Create("Strength Value", [character]() mutable { return std::to_string(character->GetStrength()); }, 2, true));
        createCharacter->AddNode(ButtonItem::Create("Strength Increment", "+", [character]() mutable { if (character->GetFreeStatPoints() > 0) { character->DecrementFreeStatPoints(std::max(character->GetStrengthModifier(), 1)); character->IncrementStrength(); } }, true, []() {return false; }));

        createCharacter->AddNode(TextItem::Create("Health", []() {return "Health       "; }, 2, false));
        createCharacter->AddNode(ButtonItem::Create("Health Decrement", "-", [character]() mutable { character->DecrementHealth(); character->IncrementFreeStatPoints(std::max(character->GetHealthModifier(), 1)); }, true, []() {return false; }));
        createCharacter->AddNode(TextItem::Create("Health Value", [character]() mutable { return std::to_string(character->GetHealth()); }, 2, true));
        createCharacter->AddNode(ButtonItem::Create("Health Increment", "+", [character]() mutable { if (character->GetFreeStatPoints() > 0) { character->DecrementFreeStatPoints(std::max(character->GetHealthModifier(), 1)); character->IncrementHealth(); } }, true, []() {return false; }));

        createCharacter->AddNode(TextItem::Create("Intellligence", []() {return "Intelligence "; }, 2, false));
        createCharacter->AddNode(ButtonItem::Create("Intelligence Decrement", "-", [character]() mutable { character->DecrementIntelligence(); character->IncrementFreeStatPoints(std::max(character->GetIntelligenceModifier(), 1)); }, true, []() {return false; }));
        createCharacter->AddNode(TextItem::Create("Intelligence Value", [character]() mutable { return std::to_string(character->GetIntelligence()); }, 2, true));
        createCharacter->AddNode(ButtonItem::Create("Intelligence Increment", "+", [character]() mutable { if (character->GetFreeStatPoints() > 0) { character->DecrementFreeStatPoints(std::max(character->GetIntelligenceModifier(), 1)); character->IncrementIntelligence(); } }, true, []() {return false; }));

        createCharacter->AddNode(TextItem::Create("Charisma", []() {return "Charisma     "; }, 2, false));
        createCharacter->AddNode(ButtonItem::Create("Charisma Decrement", "-", [character]() mutable { character->DecrementCharisma(); character->IncrementFreeStatPoints(std::max(character->GetCharismaModifier(), 1)); }, true, []() {return false; }));
        createCharacter->AddNode(TextItem::Create("Charisma Value", [character]() mutable { return std::to_string(character->GetCharisma()); }, 2, true));
        createCharacter->AddNode(ButtonItem::Create("Charisma Increment", "+", [character]() mutable { if (character->GetFreeStatPoints() > 0) { character->DecrementFreeStatPoints(std::max(character->GetCharismaModifier(), 1)); character->IncrementCharisma(); } }, true, []() {return false; }));

        createCharacter->AddNode(TextItem::Create("Leadership", []() {return "Leadership   "; }, 2, false));
        createCharacter->AddNode(ButtonItem::Create("Leadership Decrement", "-", [character]() mutable { character->DecrementLeadership(); character->IncrementFreeStatPoints(std::max(character->GetLeadershipModifier(), 1)); }, true, []() {return false; }));
        createCharacter->AddNode(TextItem::Create("Leadership Value", [character]() mutable { return std::to_string(character->GetLeadership()); }, 2, true));
        createCharacter->AddNode(ButtonItem::Create("Leadership Increment", "+", [character]() mutable { if (character->GetFreeStatPoints() > 0) { character->DecrementFreeStatPoints(std::max(character->GetLeadershipModifier(), 1)); character->IncrementLeadership(); } }, true, []() {return false; }));

        createCharacter->AddNode(TextItem::Create("Free Points", []() {return "Free Stat Points     "; }, 2, false));

        createCharacter->AddNode(TextItem::Create("Free Points Value", [character]() mutable { return std::to_string(character->GetFreeStatPoints()); }, 2, true));


        createCharacter->AddNode(TextItem::Create("New Line", []() {return " "; }, 3, false));
        createCharacter->AddNode(TextItem::Create("Inventory", []() {return "Inventory"; }, 3, false));
        createCharacter->AddNode(TextItem::Create("Coins", []() {return "Coins        "; }, 2, false));
        createCharacter->AddNode(TextItem::Create("Coins Value", [character]() mutable
            {
                int extra = character->GetCoinsEquivalentOfMissingStats(character->GetFreeStatPoints());
                return std::to_string(character->GetCoins() + extra);
            }, 2, true));

        createCharacter->AddNode(TextItem::Create("New Line", []() {return " "; }, 3, false));
        createCharacter->AddNode(ButtonItem::Create("Create Character Button", "Create", 
            [character,createCharacter]() mutable 
            {
                int extra = character->GetCoinsEquivalentOfMissingStats(character->GetFreeStatPoints());
                character->SetCoins(character->GetCoins()+extra);
                createCharacter->Hide();
            }, false, 
            [character]()mutable 
            { 
                int extra = character->GetCoinsEquivalentOfMissingStats(character->GetFreeStatPoints());
                return (character->GetFreeStatPoints() < 0) || (character->GetCoins()+extra<0); 
            }));
        createCharacter->Hide();
        return createCharacter;
	}
}