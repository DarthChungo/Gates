#ifndef SIMX_SIMXAPPLICATION_HPP
#define SIMX_SIMXAPPLICATION_HPP

#include "pch.hpp"
#include "Engine/Application.hpp"
#include "Engine/OrthographicCamera.hpp"

namespace Gates {
  class GatesApplication : public Application {
   public:
    friend class PhysicsEngine;

   private:
    rcode pOnUpdate() override;
    rcode pOnRender() override;
    rcode pOnImGuiRender() override;
    rcode pOnLaunch() override;
    rcode pOnClose() override;

    glm::vec2 MousePosWorld();

   private:
    bool dragging = false;
    bool running  = false;

    glm::vec2 offset = {};

    float              view_distance = 200.f;
    float              view_rotation = 0.f;
    OrthographicCamera camera {view_distance, 1.f};

    bool show_statistics = false;
  };
}

#endif
