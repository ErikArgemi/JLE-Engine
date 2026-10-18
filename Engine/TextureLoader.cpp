#include "TextureLoader.h"
#include "../Editor/Log.h"
#include <glad/glad.h>
#include <filesystem>

Texture TextureLoader::LoadTexture(const char* path) {
	ILuint img_ID = 0;
	ilGenImages(1, &img_ID);
	ilBindImage(img_ID);

	if (!ilLoadImage(path)) {
		LOG("TextureLoader: Couldn't load the image with path '%s'.", path);
		std::cout << "Could not load texture: " << path << " | Working directory: " << std::filesystem::current_path() << std::endl;
		ilDeleteImages(1, &img_ID);
		return Texture();
	}

	ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

	ILint img_width = ilGetInteger(IL_IMAGE_WIDTH);
	ILint img_height = ilGetInteger(IL_IMAGE_HEIGHT);

	GLuint tex_ID;
	glGenTextures(1, &tex_ID);
	glBindTexture(GL_TEXTURE_2D	, tex_ID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 0, GL_RGBA, GL_UNSIGNED_BYTE, ilGetData());

	ilDeleteImages(1, &img_ID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);

	return Texture(img_width, img_height, tex_ID);
}