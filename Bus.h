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
		ModuleType moduleType;
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

		BusData(ModuleType sourceType=ModuleType::ANY, ModuleType destinationType=ModuleType::ANY, int sourceId=-1, int destinationId=-1,
				std::vector<int> path=std::vector<int>(), DataType dataType=DataType::DataChunk, Data dataValue=Data(),
				int contextId=-1, ModuleType reservationType=ModuleType::ANY, int reservationId=-1):
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
			_frequency = frequency;
			_type = ModuleType::BUS;
		}

		void Compute() override
		{
			
		}

		void ComputePaths(Bus * root = nullptr, int jumps = 1, std::map<Module *,bool> * filter=nullptr)
		{
			if (!root)
				root = this;

			std::shared_ptr< std::map<Module*, bool>> mapPtr;			
			if (!filter)
			{
				mapPtr = std::make_shared<std::map<Module*, bool>>();
				filter = mapPtr.get();
				filter->operator[](this) = true;
			}

			for (int i = 0; i < 4; i++)
			{
				auto conn = _directConnectedModules[i];
				
				if (conn.get())
				{
					
					if (filter->find(conn.get()) == filter->end())
					{
						filter->operator[](conn.get()) = true;

						auto typ = conn->GetModuleType();
						auto id = conn->GetId();

						BusConnection bConn;
						bConn.jumps = jumps;
						bConn.moduleId = id;
						bConn.moduleType = typ;
						bConn.numRecentlyUsed = 0;
						bConn.numTotalUsed = 0;
						root->_farConnectionTypeList[typ].push_back(bConn);
						root->_farConnectionIdList[id] = bConn;
						
						if (typ == ModuleType::BUS)
						{
							conn->AsPtr<Bus>()->ComputePaths(root, jumps + 1,filter);
						}
					}

				}
			}
			
		}

		void PrintPaths()
		{
			std::cout << "far connections by type:" << std::endl;
			for (auto& e : _farConnectionTypeList)
				for (auto& f : e.second)
					std::cout << "type:" << e.first << " id:" << f.moduleId <<" jumps:"<<f.jumps << std::endl;
		}
	private:

		// 0: top, 1: right, 2: bottom, 3: left
		BusData _busRegister[4];
		



		// At the end of the day, I'm connected to these type of modules, it will take that amount of jumps to reach there
		std::map<ModuleType, std::vector<BusConnection>> _farConnectionTypeList;
		std::map<int, BusConnection> _farConnectionIdList;
	};
}