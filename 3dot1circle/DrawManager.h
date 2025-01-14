//DrawManager.h
#ifndef _DRAWMANAGER_H
#define _DRAWMANAGER_H
#include "ThreadPool.h"
#include <afxwin.h>
#include <atlimage.h>

#define DRAW_MGR DrawManager::GetInstance()

class Shape;
class DrawManager {
	
public:
	static std::shared_ptr<DrawManager> GetInstance() {
		if (m_pInstance == nullptr)
			m_pInstance = std::shared_ptr<DrawManager>(new DrawManager());
		return m_pInstance;
	}
	~DrawManager();

	void Draw(CDC* pDC, CImage* pImage, std::vector<Shape*>& shapes);
	void Erase(CImage* pImage);
	void SetThickness(int thickness) { m_nThickness = thickness; };

private:
	static std::shared_ptr<DrawManager> m_pInstance;
	DrawManager();

	ThreadPool* m_pThreadPool;
	int m_nThickness;
};


#endif //_DRAWMANAGER_H