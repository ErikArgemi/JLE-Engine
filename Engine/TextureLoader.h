#pragma once
#include <iostream>
#include <glad/glad.h>
#include "IL/il.h"

struct Texture {
	ILint width;
	ILint height;
	GLuint id;

	Texture() {
		this->id = 0;
	}

	Texture(ILint width, ILint height, GLuint id) {
		this->width = width;
		this->height = height;
		this->id = id;
	}
};

class TextureLoader {
public:
	static Texture LoadTexture(const char* path);
};