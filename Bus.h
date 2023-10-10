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


	
	// unified circular bus
	class Bus :public Module
	{
	public:
		Bus(int frequency, int lithography)
		{
			_lithography = lithography;		
			_frequency = frequency;
			_type = ModuleType::BUS;
			_busRegister[0].dataType = Design::DataType::Null;
			
		}
		void SetBusy() override { _isBusy = true; }
		void Compute() override
		{
			
			// rotate 4 data points
			auto d0 = _busRegister[0];
			auto d1 = _busRegister[1];
			auto d2 = _busRegister[2];
			auto d3 = _busRegister[3];			

			_busRegister[0] = d3;
			_busRegister[1] = d0;
			_busRegister[2] = d1;
			_busRegister[3] = d2;
			_isBusy = false;
			for (int i = 0; i < 4; i++)
			{
				if (_busRegister[i].dataType == Design::DataType::Null)
					if (_input[i].dataType != Design::DataType::Null)
					{
						SetBusy();
						_busRegister[i] = _input[i];
						_input[i] = Data();
					}
			}
			for (int i = 0; i < 4; i++)
			{
				auto reg = _busRegister[i];
				if (reg.dataType == Design::DataType::Result)
					std::cout << "zzzzzzzzzzzzz" << std::endl;
				// if has data, check if aligned at shortest module output, then send
				if (reg.dataType != Design::DataType::Null)
				{



					// check  current distance
					int curDist = 10000;
					auto farConn = GetFarConnectionsOfType(reg.targetModuleType);
					for (auto& fc : farConn)
					{
						if (curDist > fc.jumps)
						{
							curDist = fc.jumps;
						}
					}
					std::cout << "curDist=" << curDist << std::endl;

					// check shortest module
					int shortestPath = 10000;
					int shortestIdx = -1;
					for (int j = 0; j < 4; j++)
					{
						auto conn = _directConnectedModules[j];
						if (conn.get())
						{

							if (reg.targetModuleId == conn->GetId() && (i==j))
							{
								// aligned directly, send data
								int idx = 0;
								if (j == 0)
									idx = 2;
								else if (j == 1)
									idx = 3;
								else if (j == 2)
									idx = 0;
								else if (j == 3)
									idx = 1;
								conn->SetInput(reg, idx);
								_busRegister[i] = Data();
								std::cout << "bus->module" << std::endl;
								SetBusy();
								break;
							}
							else if(conn->GetModuleType() == Design::ModuleType::BUS)
							{
								// check distance
								auto connBus = conn->AsPtr<Design::Bus>();
								auto jumpList = connBus->GetFarConnectionsOfType(reg.targetModuleType);
								for (auto& jl : jumpList)
								{
									std::cout << jl.jumps << std::endl;
									if (jl.jumps < shortestPath)
									{
										shortestPath = jl.jumps;
										shortestIdx = j;
									}
								}
								
							}
						}
					}


					// compare shortest indirect distance with current distance
					if (curDist > shortestPath)
					{
						// if aligned with shortest
						if (shortestIdx == i)
						{
							int idx = 0;
							if (i == 0)
								idx = 2;
							else if (i == 1)
								idx = 3;
							else if (i == 2)
								idx = 0;
							else if (i == 3)
								idx = 1;
							_directConnectedModules[shortestIdx]->SetInput(reg, idx);
							_busRegister[i] = Data();
							_isBusy = true;
							std::cout << "bus->bus" << std::endl;
						}
					}
				}
			}
			
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

		std::vector<BusConnection> GetFarConnectionsOfType(ModuleType moduleType)
		{
			std::vector<BusConnection> result = _farConnectionTypeList[moduleType];
			return result;
		}
	private:

		// 0: top, 1: right, 2: bottom, 3: left
		Data _busRegister[4];
		



		// At the end of the day, I'm connected to these type of modules, it will take that amount of jumps to reach there
		std::map<ModuleType, std::vector<BusConnection>> _farConnectionTypeList;
		std::map<int, BusConnection> _farConnectionIdList;
	};
}