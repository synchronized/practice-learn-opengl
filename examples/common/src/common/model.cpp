
#include "common/model.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

namespace common {

namespace {
glm::mat4 convertToGlm(const aiMatrix4x4& from) {
    // Assimp matrix is row-major; glm expects column-major. Transpose after make_mat4.
    return glm::transpose(glm::make_mat4(&from.a1));
}
} // namespace

void Model::Draw(Shader& shader) {
    for(std::size_t i = 0; i < this->meshes.size(); i++) {
        this->meshes[i].Draw(shader);
    }
}

void Model::loadModel(std::string path) {
    Assimp::Importer import;
    const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs); 

    if(!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
    {
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        return;
    }
    this->directory = path.substr(0, path.find_last_of('/'));

    glm::mat4 identity = glm::mat4(1.0f);
    this->processNode(scene->mRootNode, scene, identity);
}

void Model::processNode(aiNode* node, const aiScene* scene, const glm::mat4& parentTransform) {
    glm::mat4 nodeTransform = convertToGlm(node->mTransformation);
    glm::mat4 currentTransform = parentTransform * nodeTransform;
    // 添加当前节点中的所有Mesh
    for(std::size_t i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]]; 
        this->meshes.push_back(this->processMesh(mesh, scene, currentTransform)); 
    }
    // 递归处理该节点的子孙节点
    for(std::size_t i = 0; i < node->mNumChildren; i++)
    {
        this->processNode(node->mChildren[i], scene, currentTransform);
    }
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene, const glm::mat4& transform) {
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures;

    for (std::size_t i = 0; i < mesh->mNumVertices; i++) 
    {
        Vertex vertex;
        // 处理顶点坐标、法线和纹理坐标
        glm::vec4 pos = transform * glm::vec4(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z, 1.0f);
        vertex.Position = glm::vec3(pos);

        glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(transform)));
        vertex.Normal = glm::normalize(normalMatrix * glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z));
        if (mesh->HasTextureCoords(0)) { // 纹理坐标存在
            vertex.TexCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
        }
        else {
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);
        }
        vertices.push_back(std::move(vertex));
    }
    // 处理顶点索引

    for (std::size_t i = 0; i < mesh->mNumFaces; i++) 
    {
        aiFace& face = mesh->mFaces[i];
        for(std::size_t j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    // 处理材质
    if(mesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<Texture> diffuseMaps = this->loadMaterialTextures(
                material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        std::vector<Texture> specularMaps = this->loadMaterialTextures(
                material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }

    return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName) {
    std::vector<Texture> textures;
    for(std::size_t i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        std::string tex_path = (fs::path(this->directory) / fs::path(str.C_Str())).string();
        if (textures_loaded.find(tex_path) != textures_loaded.end()) {
            continue;
        }
        Texture texture(typeName);
        texture.open(tex_path, GL_RGB);
        textures.push_back(texture);
        textures_loaded.insert(tex_path);
    }
    return textures;
}

}