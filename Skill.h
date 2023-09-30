#pragma once
#include<string>
#include<vector>

struct SkillRequirement
{
	std::string name;
	int level;
};

struct StatRequirement
{
	std::string name;
	int level;
};

class Skill
{
public:
	virtual std::vector<SkillRequirement> GetSkillRequirements() {};
	virtual std::vector<StatRequirement> GetStatRequirements() {};
private:
	std::string _name;
	std::string _description;
	int level;
};