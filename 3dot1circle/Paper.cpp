#include "Paper.h"

Paper::Paper()
{
	m_CImage = new CImage();
	m_CImage->Create(WIDTH, HEIGHT, BPP8);
	if (m_CImage->GetBPP() == BPP8)
	{
		static RGBQUAD rgb[256];
		for (unsigned int i = 0; i < 256; i++)
			rgb[i].rgbRed = rgb[i].rgbGreen = rgb[i].rgbBlue = i;
		m_CImage->SetColorTable(0, 256, rgb);
	}
	m_nWidth = m_CImage->GetWidth();
	m_nHeight = m_CImage->GetHeight();
	m_nPitch = m_CImage->GetPitch();
	m_pBits = static_cast<unsigned char*>(m_CImage->GetBits());

	m_nThickness = 0;
	m_pFocusDot = nullptr;
	m_pCircle = nullptr;
}

Paper::~Paper()
{
	if (m_CImage != 0)
		delete m_CImage;

	for (int i = 0; i < m_vecDots.size(); i++) {
		if (m_vecDots[i] != 0)
			delete m_vecDots[i];
	}

	if (m_pCircle != 0)
		delete m_pCircle;
}

void Paper::Add(Shape* shape)
{
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
	for (int i = 0; i < m_nHeight; i++) {
		for (int j = 0; j < m_nWidth; j++) {
			m_pBits[i * m_nPitch + j] = WHITE;
		}
	}

	for (int i = 0; i < m_vecDots.size(); i++) {
		if (m_vecDots[i] != 0)
			delete m_vecDots[i];
	}

	if (m_pCircle != 0) {
		delete m_pCircle;
		m_pCircle = nullptr;
	}

	m_vecDots.clear();
	m_listDots.clear();
}

void Paper::Draw(CDC* dc)
{
	int x;
	int y;
	int nCenterX;
	int nCenterY;
	int nMaxX;
	int nMaxY;
	int nRadius;
	double dDistance;

	for (int i = 0; i < m_vecDots.size(); i++)
	{
		Shape* dot = m_vecDots[i];
		nCenterX = dot->GetX();
		nCenterY = dot->GetY();

		nRadius = m_nThickness;
		nMaxX = nCenterX + nRadius;
		nMaxY = nCenterY + nRadius;
		if (nMaxX > m_nWidth) nMaxX = m_nWidth;
		if (nMaxY > m_nHeight) nMaxY = m_nHeight;

		x = nCenterX - nRadius + 1;
		if (x < 0) x = 0;
		for (x; x < nMaxX; x++) {
			y = nCenterY - nRadius + 1;
			if (y < 0) y = 0;
			for (y; y < nMaxY; y++) {
				dDistance = sqrt(pow(nCenterX - x, 2) + pow(nCenterY - y, 2));
				if (dDistance <= nRadius)
					m_pBits[y * m_nPitch + x] = BLACK;
			}
		}
	}

	if (m_pCircle != 0)
	{
		nCenterX = m_pCircle->GetX();
		nCenterY = m_pCircle->GetY();
		nRadius = m_pCircle->GetRadius();
		nMaxX = nCenterX + nRadius + m_nThickness / 2;
		nMaxY = nCenterY + nRadius + m_nThickness / 2;;
		if (nMaxX > m_nWidth) nMaxX = m_nWidth;
		if (nMaxY > m_nHeight) nMaxY = m_nHeight;

		x = nCenterX - nRadius - m_nThickness / 2;
		for (x; x < nMaxX; x++) {
			y = nCenterY - nRadius - m_nThickness / 2;
			if (y < 0) y = 0;
			for (y; y <= nMaxY; y++) {
				dDistance = sqrt(pow(nCenterX - x, 2) + pow(nCenterY - y, 2));
				if (dDistance >= nRadius - m_nThickness / 2 && dDistance <= nRadius + m_nThickness / 2)
					m_pBits[y * m_nPitch + x] = BLACK;
			}
		}
	}

	m_CImage->Draw(*dc, 0, PADDING);
}

void Paper::Erase()
{
	for (int i = 0; i < m_nHeight; i++) {
		for (int j = 0; j < m_nWidth; j++) {
			m_pBits[i * m_nPitch + j] = WHITE;
		}
	}
}

Shape* Paper::FindByPoint(CPoint point)
{
	double distance;
	Shape* pDot = nullptr;
	Shape* pResultDot = nullptr;

	for (std::list<Shape*>::iterator it = m_listDots.begin(); it != m_listDots.end(); ++it) {
		distance = sqrt(pow((*it)->GetX() - point.x, 2) + pow((*it)->GetY() - point.y, 2));
		if (distance <= m_nThickness) {
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
