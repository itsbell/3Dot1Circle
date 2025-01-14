//Math.h
#ifndef _MATH_H
#define _MATH_H
#include <afxwin.h>

class Math {
public:
	Math();
	~Math();

	static double Area(CPoint a, CPoint b, CPoint c);
	static CPoint Center(CPoint a, CPoint b);
	static double Distance(CPoint a, CPoint b);
	static int Random(int min, int max);
};
#endif //_MATH_H