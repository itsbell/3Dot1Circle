//ThreeDotOneCircleApp.cpp

#include "ThreeDotOneCircleApp.h"
#include "MainView.h"

BOOL ThreeDotOneCircleApp::InitInstance() {
	MainView mainView;
	this->m_pMainWnd = &mainView;
	mainView.DoModal();

	return TRUE;
}

ThreeDotOneCircleApp threeDotOneCircleApp;