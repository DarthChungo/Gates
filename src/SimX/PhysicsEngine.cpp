#include "SimX/PhysicsEngine.hpp"
#include "SimX/SimXApplication.hpp"
#include "Engine/Renderer.hpp"

namespace SimX {
  void PhysicsEngine::Load() {
    particles.clear();

    particles.push_back(Particle({0, 0}, {0, 0}, {0, 0}, {0, 0}, 10000, 0.005f));
    particles.push_back(Particle({100, 0}, {0, 30}, {0, 0}, {0, 0}, 100, 0.05f));
    particles.push_back(Particle({-100, 0}, {0, -30}, {0, 0}, {0, 0}, 100, 0.05f));
    particles.push_back(Particle({200, 0}, {0, 20}, {0, 0}, {0, 0}, 200, 0.05f));

    c_grav = 5.f;
    c_time = 2.f;
  }

  void PhysicsEngine::PhysicsCalculations(double dt) {
    for (auto& p1 : particles) {
      for (auto& p2 : particles) {
        if (&p1 != &p2) {
          dist = p1.pos - p2.pos;

          rad   = (dist.x * dist.x) + (dist.y * dist.y);
          force = (c_grav * p1.mass * p2.mass) / rad;
          alpha = atan(dist.y / dist.x);

          p1.force += glm::dvec2(cos(alpha), sin(alpha)) * force * (p2.pos.x >= p1.pos.x ? 1. : -1.);
          p2.force += glm::dvec2(cos(alpha), sin(alpha)) * force * (p2.pos.x >= p1.pos.x ? -1. : 1.);
        }
      }
    }

    for (auto& p : particles) {
      p.acc = p.force / p.mass;

      p.vel += (p.acc * dt * c_time);
      p.pos += (p.vel * dt * c_time);

      p.force = {0, 0};
    }
  }
}
