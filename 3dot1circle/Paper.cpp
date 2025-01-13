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
		m_listDots.push_front(shape);
	}
	else if (Circle* circle = dynamic_cast<Circle*>(shape)) {
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
	m_listDots.clear();
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

	for (std::list<Shape*>::iterator it = m_listDots.begin(); it != m_listDots.end(); ++it) {
		distance = sqrt(pow((*it)->GetX() - point.x, 2) + pow((*it)->GetY() - point.y, 2));
		if (distance <= nRange) {
			pResultDot = (*it);
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
