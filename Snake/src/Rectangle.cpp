#include "Rectangle.h"

Rectangle::Rectangle(GLfloat ix, GLfloat iy, GLfloat iwidth, GLfloat iheight, float red, float green, float blue){
	mVertices.push_back(ix);
	mVertices.push_back(iy);
	mVertices.push_back(-1.0f);
	mVertices.push_back(ix + iwidth);
	mVertices.push_back(iy);
	mVertices.push_back(-1.0f);
	mVertices.push_back(ix);
	mVertices.push_back(iy + iheight);
	mVertices.push_back(-1.0f);
	mVertices.push_back(ix + iwidth);
	mVertices.push_back(iy + iheight);
	mVertices.push_back(-1.0f);

	mIndices.push_back(0);
	mIndices.push_back(1);
	mIndices.push_back(2);
	mIndices.push_back(3);
	mIndices.push_back(1);
	mIndices.push_back(2);

	vCol = glm::vec4(red / 255, green / 255, blue / 255, 1.0f);

	x = ix;
	y = iy;
	width = iwidth;
	height = iheight;

	mVAO = 0;
	mVBO = 0;
	mEBO = 0;
}

void Rectangle::setColour(int red, int green, int blue){
	vCol = glm::vec4(red / 255, green / 255, blue / 255, 0);
}

glm::vec4 Rectangle::getColour()
{
	return vCol;
}

void Rectangle::draw(){
	if (mLoad == false) {
		initBuffers();
		mLoad = true;
	}

	glBindVertexArray(mVAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);

	glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, nullptr);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Rectangle::initBuffers(){
	glGenBuffers(1, &mVBO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(GLfloat), &mVertices[0], GL_STATIC_DRAW);
	glGenVertexArrays(1, &mVAO);
	glBindVertexArray(mVAO);
	
	//position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, NULL, NULL);

	glGenBuffers(1, &mEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(unsigned int), &mIndices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

Rectangle::~Rectangle(){
	glDeleteVertexArrays(1, &mVAO);
	glDeleteBuffers(1, &mVBO);
	glDeleteBuffers(1, &mEBO);
}
