#pragma once
#include"Skill.h"
#include"Dice.h"
#include"Equivalence.h"
class Character
{
public:
	Character()
	{	
		_freeStatPoints = 10;
		_learning = dice.D6(3);
		_rnd = dice.D6(3);
		_strength = dice.D6(3);
		_health = dice.D6(3);
		_intelligence = dice.D6(3);
		_charisma = dice.D6(3);
		_leadership = dice.D6(3);
		_coins = 2500000;
		_experience = 0;
		_level = 1;
	}

	Character(const Character & cha)
	{
		_freeStatPoints = cha._freeStatPoints;
		_learning = cha._learning;
		_rnd = cha._rnd;
		_strength = cha._strength;
		_health = cha._health;
		_intelligence = cha._intelligence;
		_charisma = cha._charisma;
		_leadership = cha._leadership;
		_coins = cha._coins;
		_experience = cha._experience;
		_level = cha._level;
		_skills = cha._skills;
	}

	Character& operator = (const Character& cha)
	{
		_freeStatPoints = cha._freeStatPoints;
		_learning = cha._learning;
		_rnd = cha._rnd;
		_strength = cha._strength;
		_health = cha._health;
		_intelligence = cha._intelligence;
		_charisma = cha._charisma;
		_leadership = cha._leadership;
		_coins = cha._coins;
		_experience = cha._experience;
		_level = cha._level;
		_skills = cha._skills;
		return *this;
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
		int total = 0;
		for (int i = 0; i < freeStatPoints; i++)
			total += Equivalence::FreeStatPointsToCredits(i);
		for (int i = 0; i < _learning; i++)
			total -= Equivalence::LearningPointsToCredits(i);
		for (int i = 0; i < _rnd; i++)
			total -= Equivalence::RndPointsToCredits(i);

		for (int i = 0; i < _strength; i++)
			total -= Equivalence::StrengthPointsToCredits(i);
		for (int i = 0; i < _health; i++)
			total -= Equivalence::HealthPointsToCredits(i);
		for (int i = 0; i < _intelligence; i++)
			total -= Equivalence::IntelligencePointsToCredits(i);
		for (int i = 0; i < _charisma; i++)
			total -= Equivalence::CharismaPointsToCredits(i);
		for (int i = 0; i < _leadership; i++)
			total -= Equivalence::LeadershipPointsToCredits(i);
		return total;
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

	int GetFreeStatPoints()
	{
		return _freeStatPoints;
	}
	void IncrementFreeStatPoints(int n = 1)
	{
		_freeStatPoints += n;
	}
	void DecrementFreeStatPoints(int n = 1)
	{
		_freeStatPoints -= n;
	}

	void SetCoins(int n)
	{
		_coins = n;
	}

	Dice dice;
private:
	// every character has own dice (for future parallelizations & fairness)

	// location in world to affect shipment price
	int _location;

	// stats
	int _freeStatPoints;
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
