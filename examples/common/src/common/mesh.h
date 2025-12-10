#ifndef _COMMON_MESH_H_
#define _COMMON_MESH_H_

#include <vector>

#include "common/vertex.h"
#include "common/texture.h"
#include "common/shader.h"

namespace common {

class Mesh
{
public:
    Mesh() = default;
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> texture);
    void Draw(Shader& shader);

public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

private:
    unsigned int VAO, VBO, EBO;
    void setupMesh();
};

}

#endif