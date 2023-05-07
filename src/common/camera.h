
#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#ifndef _CAMERA_H_
#define _CAMERA_H_

namespace common {

class Camera {
  public:
    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

    float screenWidth = 4.0f;
    float screenHeight = 3.0f;

  public:
    void toForward(float delta);
    void toBack(float delta);
    void toLeft(float delta);
    void toRight(float delta);

    glm::mat4& getView();
    glm::mat4& getProjection();
};


}

#endif
