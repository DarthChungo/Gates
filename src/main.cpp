#include "pch.hpp"
#include "SimX/SimXApplication.hpp"

int main(int, char**) {
  SimX::SimXApplication app;

  app.Construct({1000, 1000}, {0, 0}, "Gates");
  app.Launch();

  return 0;
}
