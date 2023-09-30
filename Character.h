#pragma once
#include"Skill.h"
#include"Dice.h"
#include"Equivalence.h"
class Character
{
public:
	Character()
	{
		// 3d6 for stats
		_learning = 10; 
		_rnd = 10;
		_strength = 10;
		_health = 10;
		_intelligence = 10;
		_charisma = 10;
		_leadership = 10;
		_coins = 1000;
		_experience = 0;
		_level = 1;
	}

	bool LevelupPossible()
	{
		return _experience >= std::pow(_level, 2) * 10;
	}

	void IncrementLearning(int n=1)
	{
		_learning += n;
	}

	void DecrementLearning(int n = 1)
	{
		_learning -= n;
	}

	int GetLearning()
	{
		return _learning;
	}

	void IncrementRnd(int n = 1)
	{
		_rnd += n;
	}

	void DecrementRnd(int n = 1)
	{
		_rnd -= n;
	}

	int GetRnd()
	{
		return _rnd;
	}

	void IncrementStrength(int n = 1)
	{
		_strength += n;
	}

	void DecrementStrength(int n = 1)
	{
		_strength -= n;
	}

	int GetStrength()
	{
		return _strength;
	}

	void IncrementHealth(int n = 1)
	{
		_health += n;
	}

	void DecrementHealth(int n = 1)
	{
		_health -= n;
	}

	int GetHealth()
	{
		return _health;
	}


	void IncrementIntelligence(int n = 1)
	{
		_intelligence += n;
	}

	void DecrementIntelligence(int n = 1)
	{
		_intelligence -= n;
	}

	int GetIntelligence()
	{
		return _intelligence;
	}

	void IncrementCharisma(int n = 1)
	{
		_charisma += n;
	}

	void DecrementCharisma(int n = 1)
	{
		_charisma -= n;
	}

	int GetCharisma()
	{
		return _charisma;
	}

	void IncrementLeadership(int n = 1)
	{
		_leadership += n;
	}

	void DecrementLeadership(int n = 1)
	{
		_leadership -= n;
	}

	int GetLeadership()
	{
		return _leadership;
	}

	void IncrementCoins(int n = 1)
	{
		_coins += n;
	}

	void DecrementCoins(int n = 1)
	{
		_coins -= n;
	}

	int GetCoins()
	{
		return _coins;
	}


	int GetCoinsEquivalentOfMissingStats(int freeStatPoints)
	{
		return
			Equivalence::FreeStatPointsToCredits(freeStatPoints) +
			Equivalence::LearningPointsToCredits(std::max(18 - _learning, 0)) +
			Equivalence::RndPointsToCredits(std::max(18 - _rnd, 0)) +
			Equivalence::StrengthPointsToCredits(std::max(18 - _strength, 0)) +
			Equivalence::HealthPointsToCredits(std::max(18 - _health, 0)) +
			Equivalence::IntelligencePointsToCredits(std::max(18 - _intelligence, 0)) +
			Equivalence::CharismaPointsToCredits(std::max(18 - _charisma, 0)) +
			Equivalence::LeadershipPointsToCredits(std::max(18 - _leadership, 0));		
	}

	int GetLearningModifier()
	{
		return ( (_learning >= 10) ? (_learning - 10) / 2 : -(10 - _learning + 1)/2);
	}

	int GetRndModifier()
	{
		return ((_rnd >= 10) ? (_rnd - 10) / 2 : -(10 - _rnd + 1) / 2);
	}

	int GetStrengthModifier()
	{
		return ((_strength >= 10) ? (_strength - 10) / 2 : -(10 - _strength + 1) / 2);
	}


	int GetHealthModifier()
	{
		return ((_health >= 10) ? (_health - 10) / 2 : -(10 - _health + 1) / 2);
	}


	int GetIntelligenceModifier()
	{
		return ((_intelligence >= 10) ? (_intelligence - 10) / 2 : -(10 - _intelligence + 1) / 2);
	}


	int GetCharismaModifier()
	{
		return ((_charisma >= 10) ? (_charisma - 10) / 2 : -(10 - _charisma + 1) / 2);
	}


	int GetLeadershipModifier()
	{
		return ((_leadership >= 10) ? (_leadership - 10) / 2 : -(10 - _leadership + 1) / 2);
	}

	Dice dice;
private:
	// every character has own dice (for future parallelizations & fairness)


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

	// inventory
	int _coins;
};
