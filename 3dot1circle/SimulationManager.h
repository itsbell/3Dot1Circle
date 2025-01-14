//SimulationManager.h
#ifndef _SIMULATIONMANAGER_H
#define _SIMULATIONMANAGER_H
#include <memory>
#include "ThreadPool.h"

#define SIMUL_MGR SimulationManager::GetInstance()

class Paper;
class SimulationManager {

public:
	static std::shared_ptr<SimulationManager> GetInstance() {
		if (m_pInstance == nullptr)
			m_pInstance = std::shared_ptr<SimulationManager>(new SimulationManager());
		return m_pInstance;
	}
	~SimulationManager();
	void Simulate(Paper* pPaper, int nRadius);

private:
	static std::shared_ptr<SimulationManager> m_pInstance;
	SimulationManager();

	ThreadPool* m_pThreadPool;
};


#endif //_SIMULATIONMANAGER_H