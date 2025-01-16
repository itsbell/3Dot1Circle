// MainView.cpp: 구현 파일

#include "afxdialogex.h"
#include "MainView.h"

#include "Common.h"
#include "Math.h"
#include "Paper.h"

#include "DrawManager.h"
#include "SimulationManager.h"

using namespace MAIN_VIEW_MSG;

IMPLEMENT_DYNAMIC(MainView, CDialog)

MainView::MainView(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_MAIN_VIEW, pParent)
{
	m_pPaper = nullptr;
}

MainView::~MainView()
{
}

BEGIN_MESSAGE_MAP(MainView, CDialog)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_BUTTON_RANDOM, &MainView::OnBnClickedButtonRandom)
	ON_BN_CLICKED(IDC_BUTTON_RESET, &MainView::OnBnClickedButtonReset)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

BOOL MainView::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_pPaper = new Paper();
	DRAW_MGR->Erase(m_pPaper->GetImage());

	m_CEditRadius.SetWindowText(DEFAULT_RADIUS);
	m_CEditThickness.SetWindowText(DEFAULT_THICKNESS);

	Invalidate(FALSE);

	return TRUE;
}

void MainView::RefreshViewData(int nMsg, CPoint point /* = 0*/)
{
	CString text;
	
	switch (nMsg)
	{
		case RESET:
			text = "";
			GetDlgItem(IDC_STATIC_COORDINATE)->SetWindowText(text);
			GetDlgItem(IDC_STATIC_SHAPE_COORDINATE)->SetWindowText(text);
			break;

		case COORDINATE_MOUSE:
			text.Format("(%03d,%03d)", point.x, point.y);
			GetDlgItem(IDC_STATIC_COORDINATE)->SetWindowText(text);
			break;

		case COORDINATE_SHAPE:
			Shape* shape;
			for (int i = 0; i < m_pPaper->GetShapes().size(); i++)
			{
				if (i > 0)
				{
					text.Format("%s | ", text);
				}
				shape = m_pPaper->GetShapes()[i];
				if (Dot* dot = dynamic_cast<Dot*>(shape))
				{
					text.Format("%s점%d: (%03d,%03d)", text, i + 1, shape->GetX(), shape->GetY());
				}
				else if(Circle* circle = dynamic_cast<Circle*>(shape))
				{
					text.Format("%s원: (%03d,%03d)", text, shape->GetX(), shape->GetY());
				}
			}
			GetDlgItem(IDC_STATIC_SHAPE_COORDINATE)->SetWindowText(text);
			break;

		case ENABLE_WINDOW_TRUE:
			m_CEditRadius.EnableWindow(TRUE);
			m_CEditThickness.EnableWindow(TRUE);
			m_CButtonRandom.EnableWindow(TRUE);

		default:
			break;
	}
}

CString MainView::GetErrorMsgString(ERROR_CODE nErrorCode)
{
	CString strErrorMsg;

	switch (nErrorCode)
	{
		case ERROR_RADIUS_EMPTY:		strErrorMsg = "원의 반지름을 입력해주세요.";		break;
		case ERROR_THICKNESS_EMPTY:		strErrorMsg = "원의 가장자리 두께를 입력해주세요."; break;
		case ERROR_RADIUS_ZERO:			strErrorMsg = "반지름은 0이 될 수 없어요.";			break;
		case ERROR_THICKNESS_ZERO:		strErrorMsg = "가장자리 두께는 0이 될 수 없어요.";	break;

		default: break;
	}
	return strErrorMsg;
}

void MainView::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_RADIUS, m_CEditRadius);
	DDX_Control(pDX, IDC_EDIT_THICKNESS, m_CEditThickness);
	DDX_Control(pDX, IDC_BUTTON_RANDOM, m_CButtonRandom);
}

void MainView::OnPaint()
{
	CPaintDC dc(this);

	DRAW_MGR->Erase(m_pPaper->GetImage());
	DRAW_MGR->Draw(&dc, m_pPaper->GetImage(), m_pPaper->GetShapes());
}

void MainView::OnClose()
{

	SIMUL_MGR->StopSimulation();
	std::this_thread::sleep_for(std::chrono::milliseconds(INTERVAL));
	if (m_pPaper != 0) {
		delete m_pPaper;
	}

	CDialog::EndDialog(0);
}

void MainView::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (SIMUL_MGR->IsRun() == false)
	{
		point.y -= PADDING;
		if (point.y >= 0)
		{
			bool isValid = true;
			ERROR_CODE nErrorCode;

			CString strRadius, strThickness;
			m_CEditRadius.GetWindowText(strRadius);
			m_CEditThickness.GetWindowText(strThickness);

			int nRadius = _ttoi(strRadius);
			int nThickness = _ttoi(strThickness);

			if (strRadius.IsEmpty()) {
				nErrorCode = ERROR_RADIUS_EMPTY;
				isValid = false;
			}
			else if (strThickness.IsEmpty()) {
				nErrorCode = ERROR_THICKNESS_EMPTY;
				isValid = false;
			}
			else if (nRadius == 0) {
				nErrorCode = ERROR_RADIUS_ZERO;
				isValid = false;
			}
			else if (nThickness == 0) {
				nErrorCode = ERROR_THICKNESS_ZERO;
				isValid = false;
			}

			if (isValid == true)
			{
				DRAW_MGR->SetThickness(nThickness);

				Shape* shape = nullptr;
				shape = m_pPaper->FindByPoint(point, nThickness);
				if (shape != nullptr)
					m_pPaper->SetFocusDot(shape);
				else
				{
					size_t nDotCnt = m_pPaper->GetDotCount();
					if (nDotCnt < MAX_DOTS)
					{
						shape = new Dot(point);
						m_pPaper->Add(shape);
					}

					nDotCnt = m_pPaper->GetDotCount();
					if (nDotCnt == MAX_DOTS)
				{
					CString strRadius;

					m_CEditRadius.GetWindowText(strRadius);
					int nRadius = _ttoi(strRadius);

					Shape* dot1 = m_pPaper->GetAt(0);
					Shape* dot2 = m_pPaper->GetAt(1);
					Shape* dot3 = m_pPaper->GetAt(2);
					double area = abs(dot1->GetX() * (dot2->GetY() - dot3->GetY()) + dot2->GetX() * (dot3->GetY() - dot1->GetY()) + dot3->GetX() * (dot1->GetY() - dot2->GetY())) / 2.0f;
					if (area > 0)
					{
						double dDistance = Math::Distance(CPoint(dot1->GetX(), dot1->GetY()), CPoint(dot2->GetX(), dot2->GetY()));
						if (dDistance <= nRadius * 2)
						{
							int x, y;
							double dDistance1, dDistance2;
							CPoint candidate1, candidate2;
							CPoint cpCenter = Math::Center(CPoint(dot1->GetX(), dot1->GetY()), CPoint(dot2->GetX(), dot2->GetY()));

							// 중심 후보 계산
							double h = sqrt(nRadius * nRadius - pow(dDistance / 2, 2));
							double dx = (dot2->GetY() - dot1->GetY()) / dDistance;
							double dy = (dot2->GetX() - dot1->GetX()) / dDistance;

							// 두 후보 중심
							double cx1 = cpCenter.x + h * dx;
							double cy1 = cpCenter.y - h * dy;

							double cx2 = cpCenter.x - h * dx;
							double cy2 = cpCenter.y + h * dy;

							// 세 번째 점 확인
							auto isOnCircle = [&](double cx, double cy) {
								return abs(sqrt(pow(dot3->GetX() - cx, 2) + pow(dot3->GetY() - cy, 2)) - nRadius) < 2;
								};

							if (m_pPaper->GetCircle() != nullptr)
								m_pPaper->DeleteCircle();

							if (isOnCircle(cx1, cy1)) {
								Shape* circle = new Circle(cx1, cy1, nRadius);
								m_pPaper->Add(circle);
							}
							else if (isOnCircle(cx2, cy2)) {
								Shape* circle = new Circle(cx2, cy2, nRadius);
								m_pPaper->Add(circle);
							}
						}
					}
				}
					RefreshViewData(COORDINATE_SHAPE);
					Invalidate(FALSE);
				}
			}
			else
			{
				CString strErrorMsg = GetErrorMsgString(nErrorCode);
				AfxMessageBox(strErrorMsg);
			}
		}
	}
	CDialog::OnLButtonDown(nFlags, point);
}

	void MainView::OnLButtonUp(UINT nFlags, CPoint point)
	{
		m_pPaper->SetFocusDot(nullptr);
	}

	void MainView::OnMouseMove(UINT nFlags, CPoint point)
	{
		if (point.y >= PADDING)
		{
			int x = point.x;
			int y = point.y - PADDING;
			if (x < 0) x = 0;
			if (x > m_pPaper->GetImage()->GetWidth()) x = m_pPaper->GetImage()->GetWidth();
			if (y < 0) y = 0;
			if (y > m_pPaper->GetImage()->GetHeight()) y = m_pPaper->GetImage()->GetHeight();
			point.x = x;
			point.y = y;
			RefreshViewData(COORDINATE_MOUSE, point);
		}
		if (SIMUL_MGR->IsRun() == false)
		{
			Shape* shape = m_pPaper->GetFocusDot();
			if (shape != nullptr)
				m_pPaper->Move(point);

			size_t nDotCnt = m_pPaper->GetDotCount();
			if (nDotCnt == MAX_DOTS)
			{
				CString strRadius;

				m_CEditRadius.GetWindowText(strRadius);
				int nRadius = _ttoi(strRadius);

				Shape* dot1 = m_pPaper->GetAt(0);
				Shape* dot2 = m_pPaper->GetAt(1);
				Shape* dot3 = m_pPaper->GetAt(2);
				double area = abs(dot1->GetX() * (dot2->GetY() - dot3->GetY()) + dot2->GetX() * (dot3->GetY() - dot1->GetY()) + dot3->GetX() * (dot1->GetY() - dot2->GetY())) / 2.0f;
				if (area > 0)
				{
					double dDistance = Math::Distance(CPoint(dot1->GetX(), dot1->GetY()), CPoint(dot2->GetX(), dot2->GetY()));
					if (dDistance <= nRadius * 2)
					{
						int x, y;
						double dDistance1, dDistance2;
						CPoint candidate1, candidate2;
						CPoint cpCenter = Math::Center(CPoint(dot1->GetX(), dot1->GetY()), CPoint(dot2->GetX(), dot2->GetY()));

						// 중심 후보 계산
						double h = sqrt(nRadius * nRadius - pow(dDistance / 2, 2));
						double dx = (dot2->GetY() - dot1->GetY()) / dDistance;
						double dy = (dot2->GetX() - dot1->GetX()) / dDistance;

						// 두 후보 중심
						double cx1 = cpCenter.x + h * dx;
						double cy1 = cpCenter.y - h * dy;

						double cx2 = cpCenter.x - h * dx;
						double cy2 = cpCenter.y + h * dy;

						// 세 번째 점 확인
						auto isOnCircle = [&](double cx, double cy) {
							return abs(sqrt(pow(dot3->GetX() - cx, 2) + pow(dot3->GetY() - cy, 2)) - nRadius) < 2;
							};

						if (m_pPaper->GetCircle() != nullptr)
							m_pPaper->DeleteCircle();

						if (isOnCircle(cx1, cy1)) {
							Shape* circle = new Circle(cx1, cy1, nRadius);
							m_pPaper->Add(circle);
						}
						else if (isOnCircle(cx2, cy2)) {
							Shape* circle = new Circle(cx2, cy2, nRadius);
							m_pPaper->Add(circle);
						}
					}
				}
			}
		}
		RefreshViewData(COORDINATE_SHAPE);
		InvalidateRect(CRect(0, PADDING, WIDTH, HEIGHT + PADDING), FALSE);
	}

	void MainView::OnBnClickedButtonRandom()
	{
		bool isValid = true;
		ERROR_CODE nErrorCode;

		CString strRadius, strThickness;
		m_CEditRadius.GetWindowText(strRadius);
		m_CEditThickness.GetWindowText(strThickness);

		int nRadius = _ttoi(strRadius);
		int nThickness = _ttoi(strThickness);

		if (strRadius.IsEmpty()) {
			nErrorCode = ERROR_RADIUS_EMPTY;
			isValid = false;
		}
		else if (strThickness.IsEmpty()) {
			nErrorCode = ERROR_THICKNESS_EMPTY;
			isValid = false;
		}
		else if (nRadius == 0) {
			nErrorCode = ERROR_RADIUS_ZERO;
			isValid = false;
		}
		else if (nThickness == 0) {
			nErrorCode = ERROR_THICKNESS_ZERO;
			isValid = false;
		}

		if (isValid == true)
		{
			m_CEditRadius.EnableWindow(FALSE);
			m_CEditThickness.EnableWindow(FALSE);
			m_CButtonRandom.EnableWindow(FALSE);
			m_pPaper->Clear();
			DRAW_MGR->SetThickness(nThickness);
			SIMUL_MGR->RunSimulation(m_pPaper, nRadius);
		}
		else
		{
			CString strErrorMsg = GetErrorMsgString(nErrorCode);
			AfxMessageBox(strErrorMsg);
		}
	}

	void MainView::OnBnClickedButtonReset()
	{
		SIMUL_MGR->StopSimulation();
		m_pPaper->Clear();
		RefreshViewData(ENABLE_WINDOW_TRUE);
		RefreshViewData(RESET);
		Invalidate(FALSE);
	}

