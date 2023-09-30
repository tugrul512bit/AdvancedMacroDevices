#pragma once
#include"Skill.h"
#include"Dice.h"
class Character
{
public:
	Character()
	{
		// 3d6 for stats
		_learning = dice.D6(3); 
		_rnd = dice.D6(3);
		_strength = dice.D6(3);
		_health = dice.D6(3);
		_intelligence = dice.D6(3);
		_charisma = dice.D6(3);
		_leadership = dice.D6(3);

		_experience = 0;
		_level = 1;
	}

	bool LevelupPossible()
	{
		return _experience >= std::pow(_level, 2) * 10;
	}
private:
	// every character has own dice (for future parallelizations & fairness)
	Dice dice;

	// stats
	int _learning;
	int _rnd;
	int _strength;
	int _health;
	int _intelligence;
	int _charisma;
	int _leadership;

	// skills
	std::vector<Skill> _skills;

	// basic info
	int _level;
	int _experience;
};
