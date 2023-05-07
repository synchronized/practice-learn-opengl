
#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include "common/camera.h"

namespace common {

const float speedBase = 2.5f;

void Camera::toForward(float delta) {
  float cameraSpeed = speedBase * delta; //
  cameraPos += cameraSpeed * cameraFront;
}
void Camera::toBack(float delta) {
  float cameraSpeed = speedBase * delta; //
  cameraPos -= cameraSpeed * cameraFront;
}
void Camera::toLeft(float delta) {
  float cameraSpeed = speedBase * delta; //
  cameraPos -=
      glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}
void Camera::toRight(float delta) {
  float cameraSpeed = speedBase * delta; //
  cameraPos +=
      glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

}
