#pragma once
#include"Module.h"
#include"Tech.h"
#include"Skill.h"
#include<vector>
#include<queue>
#include<cmath>
#include<map>
#include<memory>
namespace Design
{
	// connection for unified circular bus
	struct BusConnection
	{
		std::shared_ptr<ModuleType> moduleType;
		int moduleId;

		// for optimizing the shortest path as easy as possible (pick the route with least jumps)
		int jumps; 

		// optimizing least time-taking path as easy as possible (pick the route that will reach quickest)
		int numRecentlyUsed; 

		int numTotalUsed;
	};

	// data of unified circular bus
	class BusData
	{
	public:

		BusData(ModuleType sourceType=ModuleType::Any, ModuleType destinationType=ModuleType::Any, int sourceId=-1, int destinationId=-1,
				std::vector<int> path=std::vector<int>(), DataType dataType=DataType::DataChunk, Data dataValue=Data(),
				int contextId=-1, ModuleType reservationType=ModuleType::Any, int reservationId=-1):
			_sourceType(sourceType), _destinationType(destinationType),_sourceId(sourceId),_destinationId(destinationId),
			_path(path),_dataType(dataType),_dataValue(dataValue),_contextId(contextId),_reservationType(reservationType),
			_reservationId(reservationId)

		{

		}
	private:
		ModuleType _sourceType;
		ModuleType _destinationType;
		int _sourceId;
		int _destinationId;

		// controller unit fills vector with all id values of all stops the data will go through (unless a re-route happens)
		std::vector<int> _path;

		DataType _dataType;
		Data _dataValue;

		int _contextId;


		ModuleType _reservationType;
		int _reservationId;
	};
	
	// unified circular bus
	class Bus :public Module
	{
	public:
		Bus(int frequency, int lithography)
		{
			_lithography = lithography;			
		}

		void Compute() override
		{
			
		}
	private:

		// 0: top, 1: right, 2: bottom, 3: left
		BusData _busRegister[4];
		



		// At the end of the day, I'm connected to these type of modules, it will take that amount of jumps to reach there
		std::map<ModuleType, BusConnection> _farConnectionTypeList;
		std::map<int, BusConnection> _farConnectionIdList;
	};
}