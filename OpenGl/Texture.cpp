#include "Texture.h"

Texture::Texture(QOpenGLExtraFunctions* functions, const std::string& filepath) : m_filepath(filepath), m_textureId(0), m_localBuff(nullptr), m_width(0), m_height(0), m_bpp(0) {
    m_functions = functions;
    stbi_set_flip_vertically_on_load(1);
    m_localBuff = stbi_load(filepath.c_str(), &m_width, &m_height, &m_bpp, 0);
    GLenum format;
    switch(m_bpp) {
        case 1:
            format = GL_RED;
            break;
        case 3:
            format = GL_RGB;
            break;
        case 4:
            format = GL_RGBA;
            break;
    }
    if(m_localBuff) {
        m_functions->glGenTextures(1, &m_textureId);
        m_functions->glBindTexture(GL_TEXTURE_2D, m_textureId);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        m_functions->glTexImage2D(GL_TEXTURE_2D, 0, format, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, m_localBuff);
    } else {
        std::cerr << "no texture loaded" << std::endl;
    }
    
    m_functions->glBindTexture(GL_TEXTURE_2D, 0);
    if(m_localBuff) {
        stbi_image_free(m_localBuff);
    }
}

Texture::~Texture() {
    m_functions->glDeleteTextures(1, &m_textureId);
}

void Texture::Bind(unsigned int slot) {
    m_functions->glActiveTexture(GL_TEXTURE0 + slot);
    m_functions->glBindTexture(GL_TEXTURE_2D, m_textureId);
}

void Texture::Unbind() {
    m_functions->glBindTexture(GL_TEXTURE_2D, 0);
}