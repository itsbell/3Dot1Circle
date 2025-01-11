//Common.h
#ifndef _COMMON_H
#define _COMMON_H

#include <afxwin.h>
#include <vector>
#include <list>
#include <map>
#include <cmath>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Define */
#define PADDING 50
#define WIDTH 500
#define HEIGHT 500
#define BPP8 8
#define BLACK 0
#define WHITE 255

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Dot {
public:
	Dot(int x, int y);
	Dot(CPoint point);
	~Dot();
	int GetX() const;
	int GetY() const;
	void SetX(int val) { x = val; };
	void SetY(int val) { y = val; };
private:
	int x;
	int y;
};
inline int Dot::GetX() const {
	return this->x;
}
inline int Dot::GetY() const {
	return this->y;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Circle {
public:
	Circle(int x, int y, int radius);
	~Circle();

	int GetX() const;
	int GetY() const;
	int GetRadius() const;

private:
	int x;
	int y;
	int radius;
};
inline int Circle::GetX() const {
	return this->x;
}
inline int Circle::GetY() const {
	return this->y;
}
inline int Circle::GetRadius() const {
	return this->radius;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif //_COMMON_H