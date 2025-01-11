// MainView.cpp: 구현 파일

#include "afxdialogex.h"
#include "MainView.h"
#include "Paper.h"
#include "Common.h"

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
	m_pPaper->Erase();

	m_CEditRadius.SetWindowText(_T("5"));
	m_CEditThickness.SetWindowText(_T("1"));

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
	m_pPaper->Erase();
	m_pPaper->Draw(&dc);
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
			m_pPaper->SetThickness(nThickness);

			Shape* shape;
			shape = m_pPaper->FindByPoint(point);

			if (shape != nullptr)
			{
				m_pPaper->SetFocusDot(shape);
			}
			else
			{
				size_t nDotCnt = m_pPaper->GetDotCount();
				if (nDotCnt < MAX_DOTS)
				{
					shape = new Dot(point);
					m_pPaper->Add(shape);

					nDotCnt = m_pPaper->GetDotCount();
					if (nDotCnt == MAX_DOTS)
					{
						double nDistance;
						bool bIsCenter = false;
						int nCenterX = shape->GetX() - nRadius;
						int nCenterY = shape->GetY();
						for (nCenterX; nCenterX < shape->GetX() + nRadius && bIsCenter == false; nCenterX++)
						{
							bIsCenter = true;
							for (int i = 0; i < m_pPaper->GetDotCount(); i++)
							{
								Shape* dot = m_pPaper->GetAt(i);
								nDistance = sqrt(pow(nCenterX - dot->GetX(), 2) + pow(nCenterY - dot->GetY(), 2));
								if (abs(nDistance - nRadius) > 3)
								{
									bIsCenter = false;
									break;
								}
							}
							if (bIsCenter == true) {
								Shape* circle = new Circle(nCenterX, nCenterY, nRadius);
								m_pPaper->Add(circle);
								break;
							}
							if (nCenterX < shape->GetX())
								nCenterY--;
							else {
								nCenterY++;
							}
						}
						for (nCenterX; nCenterX > shape->GetX() - nRadius && bIsCenter == false; nCenterX--)
						{
							bIsCenter = true;
							for (int i = 0; i < m_pPaper->GetDotCount(); i++)
							{
								Shape* dot = m_pPaper->GetAt(i);
								nDistance = sqrt(pow(nCenterX - dot->GetX(), 2) + pow(nCenterY - dot->GetY(), 2));
								if (abs(nDistance - nRadius) > 1)
								{
									bIsCenter = false;
									break;
								}
							}
							if (bIsCenter == true) {
								if (m_pPaper->GetCircle() != 0)
								{
									delete m_pPaper->GetCircle();
									m_pPaper->SetCircle(nullptr);
								}
								Shape* circle = new Circle(nCenterX, nCenterY, nRadius);
								m_pPaper->Add(circle);
								break;
							}
							if (nCenterX > shape->GetX())
								nCenterY++;
							else {
								nCenterY--;
							}
						}
					}
					Invalidate(FALSE);
				}
			}
		}
		else
		{
			AfxMessageBox(errorMessage);
			TRACE("%d %d\n", point.x, point.y);
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
	Shape* shape = m_pPaper->GetFocusDot();
	if (shape != nullptr)
	{
		point.y -= PADDING;
		m_pPaper->Move(point);

		size_t nDotCnt = m_pPaper->GetDotCount();
		if (nDotCnt == MAX_DOTS)
		{
			CString strRadius, strThickness;
			m_CEditRadius.GetWindowText(strRadius);
			int nRadius = _ttoi(strRadius);

			double nDistance;
			bool bIsCenter = false;
			int nCenterX = shape->GetX() - nRadius;
			int nCenterY = shape->GetY();
			for (nCenterX; nCenterX < shape->GetX() + nRadius && bIsCenter == false; nCenterX++)
			{
				bIsCenter = true;
				for (int i = 0; i < m_pPaper->GetDotCount(); i++)
				{
					Shape* dot = m_pPaper->GetAt(i);
					nDistance = sqrt(pow(nCenterX - dot->GetX(), 2) + pow(nCenterY - dot->GetY(), 2));
					if (abs(nDistance - nRadius) > 1)
					{
						bIsCenter = false;
						break;
					}
				}
				if (bIsCenter == true) {
					Shape* circle = new Circle(nCenterX, nCenterY, nRadius);
					m_pPaper->Add(circle);
					break;
				}
				if (nCenterX < shape->GetX())
					nCenterY--;
				else {
					nCenterY++;
				}
			}
			for (nCenterX; nCenterX > shape->GetX() - nRadius && bIsCenter == false; nCenterX--)
			{
				bIsCenter = true;
				for (int i = 0; i < m_pPaper->GetDotCount(); i++)
				{
					Shape* dot = m_pPaper->GetAt(i);
					nDistance = sqrt(pow(nCenterX - dot->GetX(), 2) + pow(nCenterY - dot->GetY(), 2));
					if (abs(nDistance - nRadius) > 1)
					{
						bIsCenter = false;
						break;
					}
				}
				if (bIsCenter == true) {
					if (m_pPaper->GetCircle() != 0)
					{
						delete m_pPaper->GetCircle();
						m_pPaper->SetCircle(nullptr);
					}
					Shape* circle = new Circle(nCenterX, nCenterY, nRadius);
					m_pPaper->Add(circle);
					break;
				}
				if (nCenterX > shape->GetX())
					nCenterY++;
				else {
					nCenterY--;
				}
			}
			if (bIsCenter == false)
			{
				if (m_pPaper->GetCircle() != 0)
				{
					delete m_pPaper->GetCircle();
					m_pPaper->SetCircle(nullptr);
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

