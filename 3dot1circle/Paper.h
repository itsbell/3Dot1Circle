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

	void Add(Dot* dot);
	void Clear();
	void Draw(CDC* dc);
	void Erase();
	Dot* FindByPoint(CPoint point);
	void Move(CPoint point);

	int GetDotCount() const;
	Dot* GetFocusDot() const;
	int GetThickness() const;
	
	void SetThickness(int thickness) { m_nThickness = thickness; };
	void SetFocusDot(Dot* dot) { m_pFocusDot = dot; };

private:
	CImage* m_CImage;
	int m_nWidth;
	int m_nHeight;
	int m_nPitch;
	unsigned char* m_pBits;

	int m_nThickness;
	Dot* m_pFocusDot;
	std::vector<Dot*> m_vecDots;
	std::list<Dot*> m_listDotsOrder;

};

inline int Paper::GetDotCount() const {
	return m_vecDots.size();
}

inline Dot* Paper::GetFocusDot() const {
	return m_pFocusDot;
}

inline int Paper::GetThickness() const {
	return m_nThickness;
}
#endif //_PAPER_H