#pragma once

#include <string>
#include <glad/glad.h>

class Texture {
public:
    Texture(const std::string& fileName);
    ~Texture();

    //for swapping textures
    bool LoadFromFile(const std::string& fileName);

    bool Load();
    void Bind(GLenum textureUnit) const;

    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

private:
    std::string m_fileName;
    GLuint m_textureObj = 0;
};