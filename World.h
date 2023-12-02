#pragma once

#include "Class.h"
#include<vector>
#include<memory>

class Factory
{
private:
	int _ownerId;
	int _capacity;
};
class World
{
public:
private:
	size_t _startingCoins;
	// todo: add CPUs
	std::vector<CharacterClass> _vendors;
	std::vector<CharacterClass> _consumers;
	std::vector<Factory> _factories;
};