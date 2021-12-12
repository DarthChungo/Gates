#ifndef SIMX_SIMULATIONSYSTEM_HPP
#define SIMX_SIMULATIONSYSTEM_HPP

#include "pch.hpp"

namespace SimX {
  class SimXApplication;

  class Particle {
   public:
    Particle(const glm::dvec2& p, const glm::dvec2& v, const glm::dvec2& a, const glm::dvec2& f, double m, double d)
        : pos {p}, vel {v}, acc {a}, force {f}, mass {m}, dens {d} {}

    glm::dvec2 pos   = {};
    glm::dvec2 vel   = {};
    glm::dvec2 acc   = {};
    glm::dvec2 force = {};

    double dt = {};

    double mass = 0;
    double dens = 0;
  };

  class PhysicsEngine {
   public:
    PhysicsEngine(SimXApplication* app) : app(app) {}

    void Load();
    void PhysicsCalculations(double dt);

   public:
    std::vector<Particle> particles;

    double c_grav = 0, c_time = 0;
    double rad = 0, force = 0, alpha = 0;

    glm::dvec2 dist;

   private:
    SimXApplication* const app;
  };
}

#endif
