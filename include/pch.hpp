#include <iostream>
#include <string>
#include <string_view>
#include <fstream>
#include <sstream>
#include <chrono>
#include <atomic>
#include <thread>
#include <memory>
#include <vector>
#include <cstddef>
#include <cstdint>
#include <unordered_map>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>
