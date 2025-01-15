#pragma once
#include "afxdialogex.h"
#include "resource.h"

namespace MAIN_VIEW_MSG
{
	enum VIEW_MSG
	{
		RESET = 0,
		COORDINATE_MOUSE,
		COORDINATE_SHAPE,
		ENABLE_WINDOW_TRUE,
	};
}

enum ERROR_CODE
{
	ERROR_RADIUS_EMPTY = 0,
	ERROR_THICKNESS_EMPTY,
	ERROR_RADIUS_ZERO,
	ERROR_THICKNESS_ZERO,
};

class Paper;
class MainView : public CDialog
{
	DECLARE_DYNAMIC(MainView)

public:
	MainView(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~MainView();

	virtual BOOL OnInitDialog();
	void RefreshViewData(int nMsg, CPoint point = 0);

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
	CButton m_CButtonRandom;

	CString GetErrorMsgString(ERROR_CODE nErrorCode);

	DECLARE_MESSAGE_MAP()

};
