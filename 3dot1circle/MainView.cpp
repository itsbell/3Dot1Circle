// MainView.cpp: 구현 파일

#include "afxdialogex.h"
#include "MainView.h"
#include "Paper.h"
#include "Common.h"
#include "DrawManager.h"
#include "Math.h"

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

	m_CEditRadius.SetWindowText(_T("100"));
	m_CEditThickness.SetWindowText(_T("10"));

	Invalidate(FALSE);

	return TRUE;
}

void MainView::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_RADIUS, m_CEditRadius);
	DDX_Control(pDX, IDC_EDIT_THICKNESS, m_CEditThickness);
}

void MainView::OnPaint()
{
	CPaintDC dc(this);
	DRAW_MGR->Erase(m_pPaper->GetImage());
	DRAW_MGR->Draw(&dc, m_pPaper->GetImage(), m_pPaper->GetShapes());
}

void MainView::OnClose()
{
	if (m_pPaper != 0) {
		delete m_pPaper;
	}

	CDialog::EndDialog(0);
}


void MainView::OnLButtonDown(UINT nFlags, CPoint point)
{
	point.y -= PADDING;
	if (point.y >= 0)
	{
		bool isValid = true;
		CString errorMessage;

		CString strRadius, strThickness;
		m_CEditRadius.GetWindowText(strRadius);
		m_CEditThickness.GetWindowText(strThickness);

		int nRadius = _ttoi(strRadius);
		int nThickness = _ttoi(strThickness);

		if (strRadius.IsEmpty()) {
			errorMessage = _T("원의 반지름을 입력해주세요.");
			isValid = false;
		}
		else if (strThickness.IsEmpty()) {
			errorMessage = _T("원의 가장자리 두께를 입력해주세요.");
			isValid = false;
		}
		else if (nRadius == 0) {
			errorMessage = _T("반지름은 0이 될 수 없어요.");
			isValid = false;
		}
		else if (nThickness == 0) {
			errorMessage = _T("가장자리 두께는 0이 될 수 없어요.");
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
					Shape* dot1 = m_pPaper->GetAt(0);
					Shape* dot2 = m_pPaper->GetAt(1);
					Shape* dot3 = m_pPaper->GetAt(2);
					double area = Math::Area(dot1->GetPoint(), dot2->GetPoint(), dot3->GetPoint());

					if (area > 0)
					{
						double dDistance = Math::Distance(dot1->GetPoint(), dot2->GetPoint());
						if (dDistance <= nRadius * 2)
						{
							int x, y;
							double dDistance1, dDistance2;
							CPoint candidate1, candidate2;
							CPoint cpCenter = Math::Center(dot1->GetPoint(), dot2->GetPoint());

							x = cpCenter.x + (dot2->GetY() - dot1->GetY()) * (sqrt(pow(nRadius, 2) - pow(dDistance / 2, 2)) / dDistance);
							y = cpCenter.y - (dot2->GetX() - dot1->GetX()) * (sqrt(pow(nRadius, 2) - pow(dDistance / 2, 2)) / dDistance);
							candidate1 = CPoint(x, y);

							x = cpCenter.x - (dot2->GetY() - dot1->GetY()) * (sqrt(pow(nRadius, 2) - pow(dDistance / 2, 2)) / dDistance);
							y = cpCenter.y + (dot2->GetX() - dot1->GetX()) * (sqrt(pow(nRadius, 2) - pow(dDistance / 2, 2)) / dDistance);
							candidate2 = CPoint(x, y);

							dDistance1 = Math::Distance(dot3->GetPoint(), candidate1);
							dDistance2 = Math::Distance(dot3->GetPoint(), candidate2);

							if (m_pPaper->GetCircle() != nullptr)
								m_pPaper->DeleteCircle();
							if (fabs(dDistance1 - nRadius) < 2) {
								Shape* circle = new Circle(candidate1.x, candidate1.y, nRadius);
								m_pPaper->Add(circle);
							}
							else if (fabs(dDistance2 - nRadius) < 2) {
								Shape* circle = new Circle(candidate2.x, candidate2.y, nRadius);
								m_pPaper->Add(circle);
							}
						}
					}
				}
				Invalidate(FALSE);
			}
		}
		else
			AfxMessageBox(errorMessage);
	}
	CDialog::OnLButtonDown(nFlags, point);
}

void MainView::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_pPaper->SetFocusDot(nullptr);
}

void MainView::OnMouseMove(UINT nFlags, CPoint point)
{
	Shape* shape = m_pPaper->GetFocusDot();
	if (shape != nullptr)
	{
		point.y -= PADDING;
		m_pPaper->Move(point);

		size_t nDotCnt = m_pPaper->GetDotCount();
		if (nDotCnt == MAX_DOTS)
		{
			auto start = std::chrono::high_resolution_clock::now();


			CString strRadius;

			m_CEditRadius.GetWindowText(strRadius);
			int nRadius = _ttoi(strRadius);

			Shape* dot1 = m_pPaper->GetAt(0);
			Shape* dot2 = m_pPaper->GetAt(1);
			Shape* dot3 = m_pPaper->GetAt(2);
			double area = abs(dot1->GetX() * (dot2->GetY() - dot3->GetY()) + dot2->GetX() * (dot3->GetY() - dot1->GetY()) + dot3->GetX() * (dot1->GetY() - dot2->GetY())) / 2.0f;
			if (area > 0)
			{
				double dDistance = sqrt(pow(dot2->GetX() - dot1->GetX(), 2) + pow(dot2->GetY() - dot1->GetY(), 2));
				if (dDistance <= nRadius * 2)
				{
					int x, y;
					double dDistance1, dDistance2;
					CPoint candidate1, candidate2;
					CPoint cpCenter((dot1->GetX() + dot2->GetX()) / 2, (dot1->GetY() + dot2->GetY()) / 2);

					x = cpCenter.x + (dot2->GetY() - dot1->GetY()) * (sqrt(pow(nRadius, 2) - pow(dDistance / 2, 2)) / dDistance);
					y = cpCenter.y - (dot2->GetX() - dot1->GetX()) * (sqrt(pow(nRadius, 2) - pow(dDistance / 2, 2)) / dDistance);
					candidate1 = CPoint(x, y);

					x = cpCenter.x - (dot2->GetY() - dot1->GetY()) * (sqrt(pow(nRadius, 2) - pow(dDistance / 2, 2)) / dDistance);
					y = cpCenter.y + (dot2->GetX() - dot1->GetX()) * (sqrt(pow(nRadius, 2) - pow(dDistance / 2, 2)) / dDistance);
					candidate2 = CPoint(x, y);

					dDistance1 = sqrt(pow(dot3->GetX() - candidate1.x, 2) + pow(dot3->GetY() - candidate1.y, 2));
					dDistance2 = sqrt(pow(dot3->GetX() - candidate2.x, 2) + pow(dot3->GetY() - candidate2.y, 2));

					if (m_pPaper->GetCircle() != nullptr)
						m_pPaper->DeleteCircle();
					if (fabs(dDistance1 - nRadius) < 2) {
						Shape* circle = new Circle(candidate1.x, candidate1.y, nRadius);
						m_pPaper->Add(circle);
					}
					else if (fabs(dDistance2 - nRadius) < 2) {
						Shape* circle = new Circle(candidate2.x, candidate2.y, nRadius);
						m_pPaper->Add(circle);
					}
				}
			}
		}
		Invalidate(FALSE);
	}
}

void MainView::OnBnClickedButtonRandom()
{
}


void MainView::OnBnClickedButtonReset()
{
	m_pPaper->Clear();
	Invalidate(FALSE);
}

