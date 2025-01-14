#pragma once
#include "afxdialogex.h"
#include "resource.h"
#include <string>

namespace MAIN_VIEW_MSG
{
	enum VIEW_MSG
	{
		RESET = 0,
		COORDINATE_MOUSE,
		COORDINATE_SHAPE,
	};
}

enum ERROR_CODE
{
	ERROR_INVAILD_RADIUS = 0,
	ERROR_INVAILD_THICKNESS,

};

class Paper;
class MainView : public CDialog
{
	DECLARE_DYNAMIC(MainView)

public:
	MainView(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~MainView();

	virtual BOOL OnInitDialog();
	void RefreshViewData(int nMsg);
	const std::string GetErrorMsgString(ERROR_CODE nErrorCode);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	afx_msg void OnPaint();
	afx_msg void OnClose();

	/* Button */
	afx_msg void OnBnClickedButtonRandom();
	afx_msg void OnBnClickedButtonReset();

	/* Mouse */
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	

private:
	Paper* m_pPaper;
	CEdit m_CEditRadius;
	CEdit m_CEditThickness;

	DECLARE_MESSAGE_MAP()

};
