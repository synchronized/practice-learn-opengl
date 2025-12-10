#pragma once

#include <assimp/material.h>

#include "common/shader.h"
#include "common/mesh.h"
#include "common/texture.h"

#include <string>
#include <vector>
#include <set>

class aiNode; 
class aiMesh; 
class aiScene;

namespace common {

class Model 
{
public:
    /*  成员函数   */
    Model(std::string path)
    {
        this->loadModel(path);
    }
    void Draw(Shader& shader); 

private:
    /*  模型数据  */
    std::vector<Mesh> meshes;
    std::string directory;
    std::set<std::string> textures_loaded;	// Stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.

    /*  私有成员函数   */
    void loadModel(std::string path);
    void processNode(aiNode* node, const aiScene* scene, const glm::mat4& parentTransform);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene, const glm::mat4& transform);
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};

} // namespace common