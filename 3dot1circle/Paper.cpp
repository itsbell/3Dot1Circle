#include "Paper.h"

/* extern */

/************************
		 Dot
		  .
		 ...
		.....
		 ...
		  .
************************/
extern std::map<int, std::vector<int>> mapDotRange = {
	{-2,{0}},
	{-1,{-1,0,1}},
	{0,{-2,-1,0,1,2}},
	{1,{-1,0,1}},
	{2,{0}}
};

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
	for (int i = 0; i < m_vecDots.size(); i++) {
		if (m_vecDots[i] != 0)
			delete m_vecDots[i];
	}
	m_vecDots.clear();
	m_listDots.clear();

	if (m_pCircle != 0) {
		delete m_pCircle;
		m_pCircle = nullptr;
	}

	for (int i = 0; i < m_nHeight; i++) {
		for (int j = 0; j < m_nWidth; j++) {
			m_pBits[i * m_nPitch + j] = WHITE;
		}
	}
}

void Paper::Draw(CDC* dc)
{
	int dx;
	int dy;
	int startX;
	int startY;

	for (int i = 0; i < m_vecDots.size(); i++)
	{
		Shape* dot = m_vecDots[i];
		startX = dot->GetX();
		startY = dot->GetY();

		for (auto& dotRange : mapDotRange)
		{
			dy = startY - dotRange.first;

			std::vector<int>& dotXRange = dotRange.second;
			for (int j = 0; j < dotXRange.size(); j++)
			{
				dx = startX - dotXRange[j];
				if (dy >= 0 && dy < m_nHeight && dx >= 0 && dx < m_nWidth)
					m_pBits[dy * m_nPitch + dx] = BLACK;
			}
		}
	}

	if (m_pCircle != 0)
	{
		int nCenterX = m_pCircle->GetX();
		int nCenterY = m_pCircle->GetY();
		int nRadius = m_pCircle->GetRadius();
		int x = nCenterX - nRadius;
		int y = nCenterY;

		while (y >= nCenterY - nRadius - m_nThickness / 2)
		{
			x = nCenterX;
			while (x >= nCenterX - nRadius - m_nThickness / 2) {
				double dDistance = sqrt(pow(nCenterX - x, 2) + pow(nCenterY - y, 2));
				if (dDistance >= nRadius - m_nThickness / 2 && dDistance <= nRadius + m_nThickness / 2)
					m_pBits[y * m_nPitch + x] = BLACK;
				x--;
			}
			y--;
		}

		y = nCenterY - nRadius - m_nThickness / 2;
		while (y <= nCenterY)
		{
			x = nCenterX;
			while (x <= nCenterX + nRadius + m_nThickness / 2) {
				double dDistance = sqrt(pow(nCenterX - x, 2) + pow(nCenterY - y, 2));
				if (dDistance >= nRadius - m_nThickness / 2 && dDistance <= nRadius + m_nThickness / 2)
					m_pBits[y * m_nPitch + x] = BLACK;
				x++;
			}
			y++;
		}

		y = nCenterY;
		while (y <= nCenterY + nRadius + m_nThickness / 2)
		{
			x = nCenterX;
			while (x <= nCenterX + nRadius + m_nThickness / 2) {
				double dDistance = sqrt(pow(nCenterX - x, 2) + pow(nCenterY - y, 2));
				if (dDistance >= nRadius - m_nThickness / 2 && dDistance <= nRadius + m_nThickness / 2)
					m_pBits[y * m_nPitch + x] = BLACK;
				x++;
			}
			y++;
		}

		y = nCenterY + nRadius + m_nThickness / 2;
		while (y >= nCenterY)
		{
			x = nCenterX;
			while (x >= nCenterX - nRadius - m_nThickness / 2) {
				double dDistance = sqrt(pow(nCenterX - x, 2) + pow(nCenterY - y, 2));
				if (dDistance >= nRadius - m_nThickness / 2 && dDistance <= nRadius + m_nThickness / 2)
					m_pBits[y * m_nPitch + x] = BLACK;
				x--;
			}
			y--;
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
