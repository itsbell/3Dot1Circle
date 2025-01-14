//ViewManager.h
#ifndef _VIEWMANAGER_H
#define _VIEWMANAGER_H

#include <memory>

#define VIEW_MGR ViewManager::GetInstance()

class MainView;
class ViewManager {

public:
	static std::shared_ptr<ViewManager> GetInstance() {
		if (m_pInstance == nullptr)
			m_pInstance = std::shared_ptr<ViewManager>(new ViewManager());
		return m_pInstance;
	}
	~ViewManager();

	void SetMainView(MainView* pMainView) { m_pMainView = pMainView; };
	void UpdateView();

private:
	static std::shared_ptr<ViewManager> m_pInstance;
	ViewManager();

	MainView* m_pMainView;
};



#endif //_VIEWMANAGER_H