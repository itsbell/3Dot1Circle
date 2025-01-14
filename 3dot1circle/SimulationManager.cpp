//SimulationManager.cpp
#include "SimulationManager.h"
#include "Paper.h"
#include "Math.h"

#include "resource.h"
#include <chrono>
#include <thread>

std::shared_ptr<SimulationManager> SimulationManager::m_pInstance = nullptr;

SimulationManager::SimulationManager()
{
	m_pThreadPool = new ThreadPool(1);
}

SimulationManager::~SimulationManager()
{
	if (m_pThreadPool != 0)
		delete m_pThreadPool;
}

void SimulationManager::Simulate(Paper* pPaper, int nRadius)
{
	CWnd* pMainWnd = AfxGetMainWnd();
	m_pThreadPool->enqueue([=]() {
		int x, y, min, max, random;
		int width = pPaper->GetImage()->GetWidth();
		int height = pPaper->GetImage()->GetHeight();
		CPoint center, dot1, dot2, dot3;
		Shape* shape;

		for (int i = 0; i < RANDOM_ITERATION_COUNT; i++)
		{
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

			CString text;
			text.Format("점1: (%03d,%03d) | 점2: (%03d,%03d) | 점3: (%03d,%03d) | 원: (%03d,%03d)",
				pPaper->GetShapes()[0]->GetX(), pPaper->GetShapes()[0]->GetY(),
				pPaper->GetShapes()[1]->GetX(), pPaper->GetShapes()[1]->GetY(),
				pPaper->GetShapes()[2]->GetX(), pPaper->GetShapes()[2]->GetY(),
				pPaper->GetShapes()[3]->GetX(), pPaper->GetShapes()[3]->GetY());
			pMainWnd->GetDlgItem(IDC_STATIC_SHAPE_COORDINATE)->SetWindowText(text);

			std::this_thread::sleep_for(std::chrono::milliseconds(500));
		}
	});
}