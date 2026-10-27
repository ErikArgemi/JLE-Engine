#include "Texture.h"
//DevIL
#include <IL/il.h>
//OpenGL
#include <glad/glad.h>

#include <iostream>

Texture::Texture(const std::string& fileName) : m_fileName(fileName){

}

Texture::~Texture() {
    if (m_textureObj != 0) {
        glDeleteTextures(1, &m_textureObj);
        m_textureObj = 0;
    }
}

void Texture::Bind(GLenum textureUnit) const {
    glActiveTexture(textureUnit);
    glBindTexture(GL_TEXTURE_2D, m_textureObj);
}