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
					_busRegister[j][i].SetDataType(Design::DataType::Null);
			}

			// because bus
			_busynessLevelMax *= 4;
			_cloggedCycleCounter = 0;
			_uniqueLocalIdGenerator = 0;
			_deadlock = false;
			_deadlockRecoveryStep = false;
		}

		static std::shared_ptr<Module> Create(int frequency, int lithography, int parallelism)
		{
			return std::make_shared<Bus>(frequency, lithography, parallelism);
		}

		void Compute(int clockCycleId) override
		{
			SetIdle();




			// rotate 4 data points
			// if there is deadlock, rotate n more times because neighboring BUSes can be having a circular-resonance condition with current one to have deadlock
			for (int j = 0; j < (_deadlockRecoveryStep ? ((_id % 3) + 1) : 1); j++)
			{
				for (int i = 0; i < _parallelism; i++)
				{
					auto d0 = _busRegister[0][i];
					auto d1 = _busRegister[1][i];
					auto d2 = _busRegister[2][i];
					auto d3 = _busRegister[3][i];
					if (d0.GetLocalId() == -1)
						d0.SetLocalId(_uniqueLocalIdGenerator++);
					if (d1.GetLocalId() == -1)
						d1.SetLocalId(_uniqueLocalIdGenerator++);
					if (d2.GetLocalId() == -1)
						d2.SetLocalId(_uniqueLocalIdGenerator++);
					if (d3.GetLocalId() == -1)
						d3.SetLocalId(_uniqueLocalIdGenerator++);


					_busRegister[0][i] = d3;
					_busRegister[1][i] = d0;
					_busRegister[2][i] = d1;
					_busRegister[3][i] = d2;
				}
			}
			_deadlock = ComputeDeadlock();
			if (_deadlock)
				_deadlockRecoveryStep = !_deadlockRecoveryStep;

			// sort data on its priority (age)
			for (int j = 0; j < 4; j++)
			{

				// simple brute-force. todo: replace with std::sort
				for (int i = 0; i < _parallelism - 1; i++)
				{
					for (int i2 = i + 1; i2 < _parallelism; i2++)
					{
						if (_busRegister[j][i].GetClockCycleId() > _busRegister[j][i2].GetClockCycleId())
						{
							auto tmp = _busRegister[j][i];
							_busRegister[j][i] = _busRegister[j][i2];
							_busRegister[j][i2] = tmp;
						}
					}
				}
			}

			for (int i = 0; i < _parallelism; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					for (int channel = 0; channel < _parallelism; channel++)
					{
						if (_busRegister[j][/*i*/ channel].GetDataType() == Design::DataType::Null)
						{
							if (_input[j][i].GetDataType() != Design::DataType::Null)
							{
								SetBusy();

								_busRegister[j][/*i*/ channel] = _input[j][i];
								_input[j][i] = Data();
								break;
							}
						}
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
					if (_busRegister[j][i].GetDataType() != Design::DataType::Null)
					{
						table[_busRegister[j][i].GetLocalId()] = true;
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

			// sort data on its priority (age)
			for (int j = 0; j < 4; j++)
			{

				// simple brute-force. todo: replace with std::sort
				for (int i = 0; i < _parallelism - 1; i++)
				{
					for (int i2 = i + 1; i2 < _parallelism; i2++)
					{
						if (_busRegister[j][i].GetClockCycleId() > _busRegister[j][i2].GetClockCycleId())
						{
							auto tmp = _busRegister[j][i];
							_busRegister[j][i] = _busRegister[j][i2];
							_busRegister[j][i2] = tmp;
						}
					}
				}
			}

			for (int i = 0; i < _parallelism; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					auto reg = _busRegister[j][i];
					
					
					// if has data, check if aligned at shortest module output, then send
					if (reg.GetDataType() != Design::DataType::Null)
					{

						// check  current distance
						int curDist = 10000;
						auto farConn = GetFarConnectionsOfType(reg.GetTargetModuleType());
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
								// todo: Round-Robin for inputs ---> always picking first command = can't respond others = starvation !!!!!
								if (reg.GetTargetModuleId() == conn->GetId() && (j == k))
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
									bool mustBreak = false;
									for (int channel = 0; channel < conn->GetParallelism(); channel++)
									{
										if (conn->GetInput(idx, /*i*/ channel).GetDataType() == Design::DataType::Null)
										{
											conn->SetInput(reg, idx, /*i*/ channel);
											_busRegister[j][i] = Data();

											SetBusy();
											mustBreak = true;
											break;
										}
									}
									if (mustBreak)
										break;
								}
								else if (conn->GetModuleType() == Design::ModuleType::BUS)
								{
									
									// check distance
									auto connBus = conn->AsPtr<Design::Bus>(); 
									auto jumpList = connBus->GetFarConnectionsOfType(reg.GetTargetModuleType());
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
								for (int channel = 0; channel < _directConnectedModules[shortestIdx]->GetParallelism(); channel++)
								{
									if (_directConnectedModules[shortestIdx]->GetInput(idx, /*i*/ channel).GetDataType() == Design::DataType::Null)
									{
										_directConnectedModules[shortestIdx]->SetInput(reg, idx, /*i*/ channel);
										_busRegister[j][i] = Data();
										SetBusy();
										break;
									}
								}
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
		bool _deadlockRecoveryStep;
	};
}