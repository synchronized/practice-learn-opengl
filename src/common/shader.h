
#ifndef _SHADER_H_
#define _SHADER_H_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

namespace common {

class Shader {
public:
  // 程序id
  unsigned int ID;

  // 构造器
  Shader(const char *vertexPath, const char *fragmentPath);
  // 使用/激活程序
  void use();
  // uniform工具函数
  void setBool(const std::string &name, bool v1) const;
  void setInt(const std::string &name, int v1) const;
  void setFloat(const std::string &name, float v1) const;
  void setVec2(const std::string &name, float v1, float v2) const;
  void setMat4(const std::string &name, glm::mat4 &mat) const;
  void close();
};

}

#endif
