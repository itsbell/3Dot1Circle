#include "Paper.h"

Paper::Paper()
{
	m_pImage = new CImage();
	m_pImage->Create(WIDTH, HEIGHT, BPP8);
	if (m_pImage->GetBPP() == BPP8)
	{
		static RGBQUAD rgb[256];
		for (unsigned int i = 0; i < 256; i++)
			rgb[i].rgbRed = rgb[i].rgbGreen = rgb[i].rgbBlue = i;
		m_pImage->SetColorTable(0, 256, rgb);
	}

	m_pFocusDot = nullptr;
	m_pCircle = nullptr;
}

Paper::~Paper()
{
	if (m_pImage != 0)
		delete m_pImage;

	for (Shape* shape : m_vecShape) {
		if (shape != 0)
			delete shape;
	}
}

void Paper::Add(Shape* shape)
{
	m_vecShape.push_back(shape);
	if (Dot* dot = dynamic_cast<Dot*>(shape)) {
		m_vecDots.push_back(shape);
	}
	else if (Circle* circle = dynamic_cast<Circle*>(shape))
	{
		m_pCircle = shape;
	}
}

void Paper::Clear()
{
	for (Shape* shape : m_vecShape) {
		if (shape != 0)
			delete shape;
	}

	m_pFocusDot = nullptr;
	m_pCircle = nullptr;
	m_vecShape.clear();
	m_vecDots.clear();
}

void Paper::DeleteCircle()
{
	if (m_pCircle != 0)
	{
		delete m_pCircle;
		m_pCircle = nullptr;
		m_vecShape.pop_back();
	}
}

Shape* Paper::FindByPoint(CPoint point, int nRange)
{
	double distance;
	Shape* pDot = nullptr;
	Shape* pResultDot = nullptr;

	for (int i = 0; i < m_vecDots.size(); i++)
	{
		pDot = m_vecDots[i];
		distance = sqrt(pow(pDot->GetX() - point.x, 2) + pow(pDot->GetY() - point.y, 2));
		if (distance <= nRange) {
			pResultDot = pDot;
			break;
		}
	}

	return pResultDot;
}

Shape* Paper::GetAt(int index)
{
	return m_vecDots[index];
}

void Paper::Move(CPoint point)
{
	if (m_pFocusDot != nullptr)
	{
		m_pFocusDot->SetX(point.x);
		m_pFocusDot->SetY(point.y);
	}
}
