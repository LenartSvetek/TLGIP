#pragma once

#include <iostream>

#include <GL/glew.h>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

class Snake{
public:
	Snake(int ix, int vel_tabele);

	void move();
	void changeDir(bool* keys);

	int getX() { return x; }
	int getY() { return y; }
	int getScore() { return score; }
	void upScore();

	~Snake();
private:
	int x, y, score, vel_tabele;

	int direct = 0;
	enum direction {
		UP = 1,
		DOWN = 2,
		LEFT = 3,
		RIGHT = 4,
		NO = 0
	};
};

