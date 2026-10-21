#include "Texture.h"
//DevIL
#include <IL/il.h>
//OpenGL
#include <glad/glad.h>

#include <iostream>

Texture::Texture(const std::string& fileName) : m_textureTarget(textureTarget), m_fileName(fileName){

}

Texture::~Texture() {
    if (m_textureObj != 0) {
        glDeleteTextures(1, &m_textureObj);
        m_textureObj = 0;
    }
}
bool Texture::LoadFromFile(const std::string& fileName) {
    m_fileName = fileName;

    ILuint imageID = 0;
    ilGenImages(1, &imageID);
    ilBindImage(imageID);

    if (!ilLoadImage(m_fileName.c_str())) {
        std::cout << "Failed to load Texture:" << m_fileName << std::endl;
        ilDeleteImages(1, &imageID);
        return false;
    }

    ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE));

    //texID -> m_textureObj m stands for mesh
    glGenTextures(1, &m_textureObj);
    glBindTexture(GL_TEXTURE_2D, m_textureObj);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 0, GL_RGBA, GL_UNSIGNED_BYTE, ilGetData());

    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //UVs
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glGenerateMipmap(GL_TEXTURE_2D);

    // Release the CPU-side image.
    ilDeleteImages(1, &imageID);

    glBindTexture(GL_TEXTURE_2D, 0);

    return m_textureObj != 0;
}

bool Texture::Load() {
    return LoadFromFile(m_fileName);
}

void Texture::Bind(GLenum textureUnit) const {
    glActiveTexture(textureUnit);
    glBindTexture(GL_TEXTURE_2D, m_textureObj);
}