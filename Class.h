#pragma once
#include<map>
#include<string>
// character classes
// base
class CharacterClass
{
private:
	// level value --> skill id --> amount
	std::map<int, std::map<int,int>> _bonusSkillPerLevel;

	// level --> stat name --> amount
	std::map<int, std::map<std::string, int>> _bonusStatPerLevel;

	// level --> amount of coins (mostly at level 1)
	std::map<int, int> _bonusCoins;

	int _expBoostPercentage;
	int _coinBoostPercentage;
	int _levelProgressionSpeed;
public:
};

// main classes
class ProcessorArchitect :public CharacterClass
{

};

class ElectricalEngineer :public CharacterClass
{

};

class PhysicsEngineer : public CharacterClass
{

};

// the one who invented Banker's Algorithm
class Banker : public CharacterClass
{

};