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
		Bus(int frequency, int lithography, int numChannels) :
			Module(numChannels, numChannels, lithography,/*numTransistors*/ 1, ModuleType::BUS,
				/* thermalDissipationPower */ 1, frequency, /* failProbability*/ 0.0f)
		{	
			_type = ModuleType::BUS;
			for (int j = 0; j < 4; j++)
			{
				_busRegister[j].resize(_parallelism);
				for (int i = 0; i < _parallelism; i++)
					_busRegister[j][i].dataType = Design::DataType::Null;
			}

			// because bus
			_busynessLevelMax *= 4;
			_cloggedCycleCounter = 0;
			_uniqueLocalIdGenerator = 0;
			_deadlock = false;
		}

		static std::shared_ptr<Module> Create(int frequency, int lithography, int parallelism)
		{
			return std::make_shared<Bus>(frequency, lithography, parallelism);
		}

		void Compute() override
		{
			SetIdle();
			// rotate 4 data points
			for (int i = 0; i < _parallelism; i++)
			{
				auto d0 = _busRegister[0][i];
				auto d1 = _busRegister[1][i];
				auto d2 = _busRegister[2][i];
				auto d3 = _busRegister[3][i];
				if (d0.localId == -1)
					d0.localId = _uniqueLocalIdGenerator++;
				if (d1.localId == -1)
					d1.localId = _uniqueLocalIdGenerator++;
				if (d2.localId == -1)
					d2.localId = _uniqueLocalIdGenerator++;
				if (d3.localId == -1)
					d3.localId = _uniqueLocalIdGenerator++;
				_busRegister[0][i] = d3;
				_busRegister[1][i] = d0;
				_busRegister[2][i] = d1;
				_busRegister[3][i] = d2;
			}
			_deadlock = ComputeDeadlock();
			for (int i = 0; i < _parallelism; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					if (_busRegister[j][i].dataType == Design::DataType::Null)
						if (_input[j][i].dataType != Design::DataType::Null)
						{
							_busRegister[j][i] = _input[j][i];
							_input[j][i] = Data();
						}
				}
			}

		}

		bool CheckDeadlock() override
		{
			return _deadlock;
		}
		bool ComputeDeadlock()
		{
			std::map<int, bool> table;
			for (int i = 0; i < _parallelism; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					if (_busRegister[j][i].dataType != Design::DataType::Null)
					{
						table[_busRegister[j][i].localId] = true;
					}
				}
			}

			bool same = (table.size()>0);
			for (auto& t : table)
			{
				same = same && (_dataIdTable.find(t.first) != _dataIdTable.end());
			}
			if (same)
				_cloggedCycleCounter++;
			else
				_cloggedCycleCounter = 0;
			_dataIdTable = table;

			
			if (_cloggedCycleCounter > _parallelism * 4)
				return true;
			else
				return false;
		}

		void SendOutput() override
		{
			for (int i = 0; i < _parallelism; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					auto reg = _busRegister[j][i];
					auto conn = _directConnectedModules[j];
					if (conn.get())
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
						if (conn->GetInput(idx,i).dataType != Design::DataType::Null)
							continue;
					}
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


						// check shortest module
						int shortestPath = 10000;
						int shortestIdx = -1;
						for (int k = 0; k < 4; k++)
						{
							auto conn = _directConnectedModules[k]; 
							if (conn.get())
							{
							
								if (reg.targetModuleId == conn->GetId() && (j == k))
								{
									// aligned directly, send data
									
									int idx = 0;
									if (k == 0)
										idx = 2;
									else if (k == 1)
										idx = 3;
									else if (k == 2)
										idx = 0;
									else if (k == 3)
										idx = 1;
									
									conn->SetInput(reg, idx,i);
									_busRegister[j][i] = Data();
									std::cout << "bus->module " << GetId() << std::endl;
									SetBusy();
									break;
								}
								else if (conn->GetModuleType() == Design::ModuleType::BUS)
								{
									
									// check distance
									auto connBus = conn->AsPtr<Design::Bus>(); 
									auto jumpList = connBus->GetFarConnectionsOfType(reg.targetModuleType);
									for (auto& jl : jumpList)
									{
										
										if (jl.jumps < shortestPath)
										{
											shortestPath = jl.jumps; 
											shortestIdx = k;											
										}
									}

								}
							}
						}


						// compare shortest indirect distance with current distance

						if (curDist > shortestPath)
						{
						
							// if aligned with shortest
							if (shortestIdx == j)
							{
								int idx = 0;
								if (j == 0)
									idx = 2;
								else if (j == 1)
									idx = 3;
								else if (j == 2)
									idx = 0;
								else if (j == 3)
									idx = 1;
								_directConnectedModules[shortestIdx]->SetInput(reg, idx,i);
								_busRegister[j][i] = Data();
								SetBusy();
								std::cout << "bus->bus " << GetId()<<"  "<< (_directConnectedModules[shortestIdx]->GetId())<<" "<< shortestIdx<<" "<< idx << std::endl;
							}
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

			for (int j = 0; j < 4; j++)
			{
				auto conn = _directConnectedModules[j];
				
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
		// array index: 0=top, 1=right, 2=bottom, 3=left
		// vector index: channels
		// a bus is able to i/o from all channels of all directions per inner cycle (which can have frequency equal to K times the outer cyle frequency)
		std::vector<Data> _busRegister[4];
		

		// At the end of the day, I'm connected to these type of modules, it will take that amount of jumps to reach there
		std::map<ModuleType, std::vector<BusConnection>> _farConnectionTypeList;
		std::map<int, BusConnection> _farConnectionIdList;

		std::map<int, bool> _dataIdTable;
		int _cloggedCycleCounter;
		int _uniqueLocalIdGenerator;
		bool _deadlock;
	};
}