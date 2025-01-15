//Paper.h
#ifndef _PAPER_H
#define _PAPER_H
#include "Common.h"
#include <afxwin.h> 
#include <atlimage.h>

class Paper
{
public:
	Paper();
	~Paper();

	void Add(Shape* dot);
	void Clear();
	void DeleteCircle();
	Shape* FindByPoint(CPoint point, int nRange);
	Shape* GetAt(int index);
	void Move(CPoint point);

	Shape* GetCircle() const { return m_pCircle; };
	size_t GetDotCount() const { return m_vecDots.size(); };
	Shape* GetFocusDot() const { return m_pFocusDot; };
	CImage* GetImage() const { return m_pImage; };
	std::vector<Shape*>& GetShapes() { return m_vecShape; };
	
	void SetCircle(Shape* shape) { m_pCircle = shape; };
	void SetFocusDot(Shape* dot) { m_pFocusDot = dot; };

private:
	std::vector<Shape*> m_vecShape;
	std::vector<Shape*> m_vecDots;
	
	CImage* m_pImage;
	Shape* m_pFocusDot;
	Shape* m_pCircle;
};

#endif //_PAPER_H