#pragma once
#include<cmath>
namespace Equivalence
{
	static int FreeStatPointsToCredits(int stat)
	{
		return std::pow(1.25,stat) * 100;
	}

	static int StrengthPointsToCredits(int stat)
	{
		return stat * 1500;
	}

	static int IntelligencePointsToCredits(int stat)
	{
		return std::pow(1.8,stat) * 50;
	}

	static int LearningPointsToCredits(int stat)
	{
		return std::pow(1.6, stat) * 60;
	}

	static int RndPointsToCredits(int stat)
	{
		return std::pow(1.5, stat) * 70;
	}

	static int HealthPointsToCredits(int stat)
	{
		return std::pow(1.1, stat) * 30;
	}

	static int CharismaPointsToCredits(int stat)
	{
		return std::pow(2.1, stat) * 20;
	}

	static int LeadershipPointsToCredits(int stat)
	{
		return std::pow(2.2, stat) * 10;
	}
};