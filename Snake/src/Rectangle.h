#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <GL/glew.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>

#include <vector>
#include <iostream>

class Rectangle {
public:
	Rectangle(GLfloat ix, GLfloat iy, GLfloat iwidth, GLfloat iheight, float red = 255, float green = 255, float blue = 255);

	void setColour(int red, int green, int blue);
	glm::vec4 getColour();

	void initBuffers();

	void draw();

	~Rectangle();

protected:
	GLfloat x, y, width, height;

	bool mLoad = false;

	GLuint mVAO, mVBO, mEBO;

	glm::vec4 vCol;

	std::vector<GLfloat> mVertices;
	std::vector<unsigned int> mIndices;

};

#endif