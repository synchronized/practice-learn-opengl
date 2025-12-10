
#ifndef _COMMON_VERTEX_H_
#define _COMMON_VERTEX_H_

#include <glm/glm.hpp>

namespace common {

struct Vertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

}

#endif