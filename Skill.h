#pragma once
#include<string>
#include<vector>

struct SkillRequirement
{
	std::string name;
	int level;
	SkillRequirement(std::string namePrm="", int levelPrm=0) :name(namePrm), level(levelPrm) {}
};

struct StatRequirement
{
	std::string name;
	int level;
	StatRequirement(std::string namePrm = "", int levelPrm = 0) :name(namePrm), level(levelPrm) {}
};

class Skill
{
public:
	virtual std::vector<SkillRequirement> GetSkillRequirements() { return  std::vector<SkillRequirement>(); };
	virtual std::vector<StatRequirement> GetStatRequirements() { return std::vector<StatRequirement>(); };
	std::string GetName() { return _name; }
protected:
	Skill(std::string name = "", std::string description="", int level = 0) { _name = name; _description=description, _level = level; }
	std::string _name;
	std::string _description;
	int _level;
};

class MathsSkill :public Skill
{
public:
	MathsSkill() 
	{
		_name = "Maths";
		_description = "Used in everything.";
		_level = 0;
	}
	std::vector<SkillRequirement> GetSkillRequirements() override
	{
		std::vector<SkillRequirement> result;		
		return result;
	};
	std::vector<StatRequirement> GetStatRequirements() override 
	{
		std::vector<StatRequirement> result;
		return result;
	};
};

class ElectronicsSkill:public Skill
{
	std::vector<SkillRequirement> GetSkillRequirements() override 
	{
		std::vector<SkillRequirement> result;
		result.push_back(SkillRequirement(MathsSkill().GetName(), 1));
		return result;
	};
	std::vector<StatRequirement> GetStatRequirements() override 
	{
		std::vector<StatRequirement> result;
		result.push_back(StatRequirement("Intelligence", 1));
		return result;
	};
};