#include "pch.hpp"
#include "Gates/GatesApplication.hpp"

int main(int, char**) {
  Gates::GatesApplication app;

  app.Construct({1000, 1000}, {0, 0}, "Gates");
  app.Launch();

  return 0;
}
