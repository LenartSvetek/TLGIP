#ifndef TEXTURE2D_H
#define TEXTURE2D_H

#include <GL/glew.h>
#include <STB_IMAGE/stb_image.h>

#include <string>
#include <iostream>
using std::string;

class Texture2D {
public:
	Texture2D();

	bool loadTexture(const string& filename, bool generateMipMaps = true);
	void bind(GLuint texUnit = 0);
	void unbind(GLuint texUnit = 0);

	virtual ~Texture2D();

private:
	GLuint mTexture;
};

#endif
