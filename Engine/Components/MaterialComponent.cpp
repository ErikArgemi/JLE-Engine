#include "MaterialComponent.h"
#include "../../Editor/Log.h"
#include <filesystem>

void MaterialComponent::LoadTexture(const char* path) {
	if (path == nullptr) return CreateDefaultTexture();

	ILuint img_ID = 0;
	ilGenImages(1, &img_ID);
	ilBindImage(img_ID);

	if (!ilLoadImage(path)) {
		LOG("TextureLoader: Couldn't load the image with path '%s'.", path);
		std::cout << "Could not load texture: " << path << " | Working directory: " << std::filesystem::current_path() << std::endl;
		ilDeleteImages(1, &img_ID);
		CreateDefaultTexture();
		return;
	}

	ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

	ILint img_width = ilGetInteger(IL_IMAGE_WIDTH);
	ILint img_height = ilGetInteger(IL_IMAGE_HEIGHT);

	GLuint tex_ID;
	glGenTextures(1, &tex_ID);
	glBindTexture(GL_TEXTURE_2D, tex_ID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 0, GL_RGBA, GL_UNSIGNED_BYTE, ilGetData());

	ilDeleteImages(1, &img_ID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);

	width = img_width;
	height = img_height;
	id = img_ID;
}

void MaterialComponent::CreateDefaultTexture() {
	GLubyte white[4] = { 255, 255, 255, 255 };

	width = 1;
	height = 1;

	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, white);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);
}