#include "pch.hpp"
#include "Gates/GatesApplication.hpp"

int main(int, char**) {
  Gates::GatesApplication app;

  app.Construct({1000, 1000}, {0, 0}, "Gates", {.1f, .1f, .1f, 1.f});
  app.Launch();

  return 0;
}
