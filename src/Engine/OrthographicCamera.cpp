#include "Engine/OrthographicCamera.hpp"

namespace SimX {
  OrthographicCamera::OrthographicCamera(float left, float top, float right, float bottom)
      : pProjectionMatrix(glm::ortho(left, right, bottom, top, -1.f, 1.f)), pViewMatrix(1.f) {
    pViewProjectionMatrix = pProjectionMatrix * pViewMatrix;
  }

  OrthographicCamera::OrthographicCamera(float view_distance, float aspect_ratio)
      : pProjectionMatrix(glm::ortho(-view_distance * aspect_ratio,
                                     view_distance * aspect_ratio,
                                     view_distance,
                                     -view_distance,
                                     -1.f,
                                     1.f)),
        pViewMatrix(1.f) {
    pViewProjectionMatrix = pProjectionMatrix * pViewMatrix;
  }

  void OrthographicCamera::setProjection(float left, float right, float bottom, float top) {
    pProjectionMatrix     = glm::ortho(left, right, bottom, top, -1.f, 1.f);
    pViewProjectionMatrix = pProjectionMatrix * pViewMatrix;
  }

  void OrthographicCamera::setProjection(float view_distance, float aspect_ratio) {
    setProjection(-view_distance * aspect_ratio, view_distance * aspect_ratio, view_distance, -view_distance);
  }

  const glm::vec3& OrthographicCamera::getPosition() const { return pPosition; }
  void             OrthographicCamera::setPosition(const glm::vec3& position) {
    pPosition = position;
    pRecalculateViewMatrix();
  }
  void OrthographicCamera::offsetPosition(const glm::vec3& offset) {
    pPosition += offset;
    pRecalculateViewMatrix();
  }

  float OrthographicCamera::getRotation() const { return pRotation; }
  void  OrthographicCamera::setRotation(float rotation) {
    pRotation = rotation;
    pRecalculateViewMatrix();
  }

  const glm::mat4& OrthographicCamera::getProjectionMatrix() const { return pProjectionMatrix; }
  const glm::mat4& OrthographicCamera::getViewMatrix() const { return pViewMatrix; }
  const glm::mat4& OrthographicCamera::getViewProjectionMatrix() const { return pViewProjectionMatrix; }

  glm::vec2 OrthographicCamera::ScreenToWorld(const glm::vec2& pos,
                                              const float&     view_distance,
                                              const float&     aspect_ratio) {
    return pos * view_distance * glm::vec2(aspect_ratio, 1.f) + glm::vec2(pPosition.x, pPosition.y);
  }

  void OrthographicCamera::pRecalculateViewMatrix() {
    glm::mat4 transform = glm::translate(glm::mat4(1.f), pPosition) *
                          glm::rotate(glm::mat4(1.f), glm::radians(pRotation), glm::vec3(.0f, .0f, 1.f));

    pViewMatrix           = glm::inverse(transform);
    pViewProjectionMatrix = pProjectionMatrix * pViewMatrix;
  }
}
