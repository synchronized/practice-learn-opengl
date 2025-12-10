
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
    unsigned int m_textureId{0};
    unsigned int m_textureSlot;
    unsigned int m_format{GL_RGB};
    unsigned int m_wrapType_s{GL_REPEAT};
    unsigned int m_wrapType_t{GL_REPEAT};
    unsigned int m_minFilter{GL_LINEAR_MIPMAP_LINEAR};
    unsigned int m_magFilter{GL_LINEAR};
    std::string m_type;
    std::string m_path;

    Texture() = default;
    Texture(std::string tex_type) 
        : m_type(tex_type) {}
    Texture(unsigned int textureSlot) 
        : m_textureSlot(textureSlot) {}

    Texture& setFormat(unsigned int format) {m_format=format; return *this;}
    Texture& setWrapS(unsigned int wrapType) {m_wrapType_s = wrapType; return *this;}
    Texture& setWrapT(unsigned int wrapType) {m_wrapType_t = wrapType; return *this;}
    Texture& setMinFilter(unsigned int filterType) {m_minFilter = filterType; return *this;}
    Texture& setMagFilter(unsigned int filterType) {m_magFilter = filterType; return *this;}
    Texture& setType(std::string tex_type) { m_type = tex_type; return *this;}
    int getIndex() const {return m_textureSlot-GL_TEXTURE0;}

    int open(const std::string &texPath, unsigned int format);
    void close();
    void use();
};

struct TexturePathLess {
    bool operator()(const Texture& a, const Texture& b) const {
        return a.m_path < b.m_path;
    } 
};

}

#endif
