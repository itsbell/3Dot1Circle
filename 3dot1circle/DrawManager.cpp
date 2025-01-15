//DrawManager.cpp
#include "DrawManager.h"
#include "Common.h"
#include <atlimage.h>


std::shared_ptr<DrawManager> DrawManager::m_pInstance = nullptr;

DrawManager::DrawManager()
{
}

DrawManager::~DrawManager()
{
}

void DrawManager::Draw(CDC* pDC, CImage* pImage, std::vector<Shape*>& shapes)
{
	int x;
	int y;
	int nCenterX;
	int nCenterY;
	int nMaxX;
	int nMaxY;
	int nRadius;
	double dDistance;
	Shape* shape;

	int nWidth = pImage->GetWidth();
	int nHeight = pImage->GetHeight();
	int nPitch = pImage->GetPitch();
	unsigned char* pBits = static_cast<unsigned char*>(pImage->GetBits());

	for (size_t i = 0; i < shapes.size(); i++)
	{
		shape = shapes[i];
		if (Dot* pDot = dynamic_cast<Dot*>(shape))
		{
			nCenterX = pDot->GetX();
			nCenterY = pDot->GetY();

			nRadius = m_nThickness;
			if (nRadius == 1) nRadius = 2;
			nMaxX = nCenterX + nRadius;
			nMaxY = nCenterY + nRadius;
			if (nMaxX > nWidth) nMaxX = nWidth;
			if (nMaxY > nHeight) nMaxY = nHeight;

			x = nCenterX - nRadius + 1;
			if (x < 0) x = 0;
			for (x; x < nMaxX; x++) {
				y = nCenterY - nRadius + 1;
				if (y < 0) y = 0;
				for (y; y < nMaxY; y++) {
					dDistance = sqrt(pow(nCenterX - x, 2) + pow(nCenterY - y, 2));
					if (dDistance <= nRadius)
						pBits[y * nPitch + x] = BLACK;
				}
			}

		}
		else if (Circle* pCircle = dynamic_cast<Circle*>(shape))
		{
			int nHalfThickness = m_nThickness / 2;
			if (nHalfThickness == 0) nHalfThickness = 1;
			nCenterX = pCircle->GetX();
			nCenterY = pCircle->GetY();
			nRadius = pCircle->GetRadius();
			nMaxX = nCenterX + nRadius + nHalfThickness;
			nMaxY = nCenterY + nRadius + nHalfThickness;
			if (nMaxX > nWidth) nMaxX = nWidth;
			if (nMaxY > nHeight) nMaxY = nHeight;

			x = nCenterX - nRadius - nHalfThickness;
			for (x; x < nMaxX; x++) {
				if (x < 0) x = 0;
				y = nCenterY - nRadius - nHalfThickness;
				if (y < 0) y = 0;
				for (y; y < nMaxY; y++) {
					dDistance = sqrt(pow(nCenterX - x, 2) + pow(nCenterY - y, 2));
					if (dDistance >= nRadius - nHalfThickness && dDistance <= nRadius + nHalfThickness)
						pBits[y * nPitch + x] = BLACK;
				}
			}
		}
	}
	pImage->Draw(*pDC, 0, PADDING);
}

void DrawManager::Erase(CImage* pImage)
{
	int nWidth = pImage->GetWidth();
	int nHeight = pImage->GetHeight();
	int nPitch = pImage->GetPitch();
	unsigned char* pBits = static_cast<unsigned char*>(pImage->GetBits());

	for (int i = 0; i < nHeight; i++) {
		for (int j = 0; j < nWidth; j++) {
			pBits[i * nPitch + j] = WHITE;
		}
	}
}
