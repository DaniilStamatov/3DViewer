#pragma once
#include <string>
#include <QOpenGLExtraFunctions>
#include <iostream>
#include "stb_image.h"    
class Texture {
public:
    Texture(QOpenGLExtraFunctions* functions, const std::string& filepath);
    ~Texture();

    void Bind(unsigned int slot = 0);
    void Unbind();
    int GetWidth() const;
    int GetHeight() const;
private:
    std::string m_filepath;
    unsigned int m_textureId;
    unsigned char* m_localBuff;
    int m_width, m_height, m_bpp; // m_bpp - bits per pixel
    QOpenGLExtraFunctions* m_functions;
};