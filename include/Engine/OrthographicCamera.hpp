#ifndef SIMX_ORTHOGRAPHICCAMERA_HPP
#define SIMX_ORTHOGRAPHICCAMERA_HPP

#include "pch.hpp"

namespace Gates {
  class OrthographicCamera {
   public:
    OrthographicCamera(float left, float top, float right, float bottom);
    OrthographicCamera(float view_distance, float aspect_ratio);

    void setProjection(float left, float right, float bottom, float top);
    void setProjection(float view_distance, float aspect_ratio);

    const glm::vec3& getPosition() const;
    void             setPosition(const glm::vec3& position);
    void             offsetPosition(const glm::vec3& offset);

    float getRotation() const;
    void  setRotation(float rotation);

    const glm::mat4& getProjectionMatrix() const;
    const glm::mat4& getViewMatrix() const;
    const glm::mat4& getViewProjectionMatrix() const;

    glm::vec2 ScreenToWorld(const glm::vec2& pos, const float& view_distance, const float& aspect_ratio);

   private:
    void pRecalculateViewMatrix();

   private:
    glm::mat4 pProjectionMatrix;
    glm::mat4 pViewMatrix;
    glm::mat4 pViewProjectionMatrix;

    glm::vec3 pPosition = {0.0f, 0.0f, 0.0f};
    float     pRotation = 0.0f;
  };
}

#endif
