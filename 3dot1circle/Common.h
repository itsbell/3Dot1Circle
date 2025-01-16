//Common.h
#ifndef _COMMON_H
#define _COMMON_H

#include <afxwin.h>
#include <vector>
#include <list>
#include <map>
#include <cmath>
#include <chrono>
#include <iostream>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Define */
#define PADDING 50
#define WIDTH 500
#define HEIGHT 500
#define BPP8 8
#define BLACK 0
#define WHITE 255
#define MAX_DOTS 3
#define DOT_SIZE_RATIO 1.5F
#define RANDOM_ITERATION_COUNT 10
#define INTERVAL 500

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Shape {

public:
	Shape();
	virtual ~Shape() = 0;

	virtual int GetX() const = 0;
	virtual int GetY() const = 0;
	virtual CPoint GetPoint() { return CPoint(); };
	virtual int GetRadius() const { return 0; };
	virtual void SetX(int val) = 0;
	virtual void SetY(int val) = 0;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Dot : public Shape {

public:
	Dot(int x, int y);
	Dot(CPoint point);
	~Dot();

	int GetX() const override { return x; };
	int GetY() const override { return y; };
	CPoint GetPoint() { return CPoint(x,y); };
	void SetX(int val) override { x = val; };
	void SetY(int val) override { y = val; };

private:
	int x;
	int y;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Circle : public Shape {

public:
	Circle(int x, int y, int radius);
	~Circle();

	int GetRadius() const override { return radius; };
	int GetX() const override { return x; };
	int GetY() const override { return y; };
	void SetX(int val) override { x = val; };
	void SetY(int val) override { y = val; };
	void SetRadius(int val)  { radius = val; };

private:
	int x;
	int y;
	int radius;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif //_COMMON_H