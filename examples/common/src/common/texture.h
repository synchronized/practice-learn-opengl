
#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>

#ifndef _TEXTURE_H_
#define _TEXTURE_H_

namespace common {

class Texture {
  public:
    unsigned int m_textureId;
    unsigned int m_textureSlot;
    unsigned int m_format;
    unsigned int m_wrapType_s;
    unsigned int m_wrapType_t;
    unsigned int m_minFilter;
    unsigned int m_magFilter;

    Texture(unsigned int textureSlot): m_textureId(0),
                                       m_textureSlot(textureSlot),
                                       m_format(GL_RGB),
                                       m_wrapType_s(GL_REPEAT),
                                       m_wrapType_t(GL_REPEAT),
                                       m_minFilter(GL_LINEAR_MIPMAP_LINEAR),
                                       m_magFilter(GL_NEAREST_MIPMAP_NEAREST) {}

    Texture& setFormat(unsigned int format) {m_format=format; return *this;}
    Texture& setWrapS(unsigned int wrapType) {m_wrapType_s = wrapType; return *this;}
    Texture& setWrapT(unsigned int wrapType) {m_wrapType_t = wrapType; return *this;}
    Texture& setMinFilter(unsigned int filterType) {m_minFilter = filterType; return *this;}
    Texture& setMagFilter(unsigned int filterType) {m_magFilter = filterType; return *this;}
    int getIndex() const {return m_textureSlot-GL_TEXTURE0;}

    int open(const std::string &texPath, unsigned int format);
    void close();
    void use();
};

}

#endif
