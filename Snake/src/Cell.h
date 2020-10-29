#pragma once

#include "Rectangle.h"

class Cell : public Rectangle{
public:
	Cell(GLfloat ix, GLfloat iy, GLfloat iwidth);

	void setMode(std::string mode);
	std::string getMode() { return mode; }
	int getHP() { return HP; }

	void setHP(int score);

	void update();

	~Cell();
private:
	int HP = 0;

	std::string mode;
};

