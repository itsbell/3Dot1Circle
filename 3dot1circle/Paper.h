//Paper.h
#ifndef _PAPER_H
#define _PAPER_H
#include <afxwin.h> 
#include <atlimage.h>

class Paper
{
public:
	Paper(CRect rect);
	~Paper();

	void Draw(CClientDC* dc);
	void Erase();

	CImage* GetCImage() const;

private:
	CImage* m_CImage;
};

inline CImage* Paper::GetCImage() const {
	return this->m_CImage;
}
#endif //_PAPER_H