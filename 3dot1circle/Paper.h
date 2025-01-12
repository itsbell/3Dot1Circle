//Paper.h
#ifndef _PAPER_H
#define _PAPER_H
#include <afxwin.h> 
#include <atlimage.h>
#include "Common.h"

class Paper
{
public:
	Paper();
	~Paper();

	void Add(Shape* dot);
	void Clear();
	void Draw(CDC* dc);
	void Erase();
	Shape* FindByPoint(CPoint point);
	Shape* GetAt(int index);
	void Move(CPoint point);

	Shape* GetCircle() const { return m_pCircle; };
	size_t GetDotCount() const { return m_vecDots.size(); };
	Shape* GetFocusDot() const { return m_pFocusDot; };
	int GetHeight() const { return m_nHeight; };
	int GetThickness() const { return m_nThickness; };
	int GetWidth() const { return m_nWidth; }
	
	void SetCircle(Shape* shape) { m_pCircle = shape; };
	void SetThickness(int thickness) { m_nThickness = thickness; };
	void SetFocusDot(Shape* dot) { m_pFocusDot = dot; };

private:
	CImage* m_CImage;
	int m_nWidth;
	int m_nHeight;
	int m_nPitch;
	unsigned char* m_pBits;

	int m_nThickness;
	Shape* m_pFocusDot;
	std::vector<Shape*> m_vecDots;
	std::list<Shape*> m_listDots; // z-order

	Shape* m_pCircle;
};

#endif //_PAPER_H