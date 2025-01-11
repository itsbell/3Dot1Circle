//DrawingManager.h
#ifndef _DRAWINGMANAGER_H
#define _DRAWINGMANAGER_H

class DrawingManager {
public:
	DrawingManager();
	~DrawingManager();

	void SetThickness(int thickness) { m_dThickness = thickness; };

private:
	int m_dThickness;
};

#endif //_DRAWINGMANAGER_H