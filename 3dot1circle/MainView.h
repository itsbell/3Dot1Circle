#pragma once
#include "afxdialogex.h"
#include "resource.h"

class Paper;
class MainView : public CDialog
{
	DECLARE_DYNAMIC(MainView)

public:
	enum { IDD = IDD_MAIN_VIEW };

public:
	MainView(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~MainView();

	virtual BOOL OnInitDialog();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	afx_msg void OnPaint();
	afx_msg void OnClose();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedButtonRandom();
	afx_msg void OnBnClickedButtonReset();

private:
	Paper* m_pPaper;
	CEdit m_CEditRadius;
	CEdit m_CEditThickness;

	DECLARE_MESSAGE_MAP()

};
