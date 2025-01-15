//SimulationManager.cpp
#include "SimulationManager.h"
#include "Paper.h"
#include "Math.h"
#include "MainView.h"

#include "resource.h"
#include <chrono>
#include <thread>

std::shared_ptr<SimulationManager> SimulationManager::m_pInstance = nullptr;

SimulationManager::SimulationManager()
	:m_bIsRun(false), m_bStop(false) {
	m_pThreadPool = new ThreadPool(1);
}

SimulationManager::~SimulationManager()
{
	if (m_pThreadPool != 0)
		delete m_pThreadPool;
}

void SimulationManager::RunSimulation(Paper* pPaper, int nRadius)
{
	m_bIsRun = true;
	m_bStop = false;
	CWnd* pMainWnd = AfxGetMainWnd();
	m_pThreadPool->enqueue([=]() {
		int x, y, min, max, random;
		int width = pPaper->GetImage()->GetWidth();
		int height = pPaper->GetImage()->GetHeight();
		CPoint center, dot1, dot2, dot3;
		Shape* shape;

		for (int i = 0; i < RANDOM_ITERATION_COUNT; i++)
		{
			if (m_bStop) break;
			pPaper->Clear();
			x = Math::Random(0, width - 1);
			y = Math::Random(0, height - 1);
			center = CPoint(x, y);

			min = center.x - nRadius;
			if (min < 0) min = 0;
			max = center.x + nRadius - 1;
			if (max >= width) max = width - 1;

			for (int j = 0; j < MAX_DOTS; j++)
			{
				x = Math::Random(min, max);
				random = Math::Random(0, 1);
				if (random == 0)
				{
					y = center.y + sqrt(pow(nRadius, 2) - pow(x - center.x, 2));
					if (y >= height)
						y = center.y - sqrt(pow(nRadius, 2) - pow(x - center.x, 2));
				}
				else
				{
					y = center.y - sqrt(pow(nRadius, 2) - pow(x - center.x, 2));
					if (y < 0)
						y = center.y + sqrt(pow(nRadius, 2) - pow(x - center.x, 2));
				}

				shape = new Dot(x, y);
				pPaper->Add(shape);
			}
			shape = new Circle(center.x, center.y, nRadius);
			pPaper->Add(shape);

			if (pMainWnd)
				pMainWnd->Invalidate();

			dynamic_cast<MainView*>(pMainWnd)->RefreshViewData(MAIN_VIEW_MSG::COORDINATE_SHAPE);
			if(i < RANDOM_ITERATION_COUNT - 1)
				std::this_thread::sleep_for(std::chrono::milliseconds(INTERVAL));
		}
		m_bStop = true;
		m_bIsRun = false;
		dynamic_cast<MainView*>(pMainWnd)->RefreshViewData(MAIN_VIEW_MSG::ENABLE_WINDOW_TRUE);
	});
}

void SimulationManager::StopSimulation()
{
	m_bStop = true;
	m_bIsRun = false;
}

