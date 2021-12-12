#include "pch.hpp"
#include "Engine/Application.hpp"
#include "Util/Logger.hpp"

namespace Gates {
  static void glfw_error_callback(int error, const char* description) {
    Logger::Die("GLFW error code " + std::to_string(error) + ": " + std::string(description));
  }

  void Application::Construct(const glm::uvec2& size,
                              const glm::uvec2& position,
                              const char*       name,
                              glm::vec4         clear_color) {
    pWindowSize = size;
    pWindowPos  = position;
    pWindowName = name;
    pClearColor = clear_color;

    pHasBeenConstructed = true;
  }

  void Application::Launch(bool background) {
    if (pHasBeenClosed) Logger::Die("Trying to launch an application which has already been closed");
    if (!pHasBeenConstructed) Logger::Die("Trying to launch an application without constructing it first");

    std::thread thread(&Application::pPlatformThread, this);

    if (background) {
      thread.detach();

    } else {
      thread.join();
    }
  }

  void Application::Close() { pWantsToClose = true; }

  void Application::EnsureClosed() {
    while (!pHasBeenClosed) {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
  }

  const glm::uvec2& Application::WindowSize() const { return pWindowSize; }
  const glm::uvec2& Application::WindowPos() const { return pWindowPos; }
  const glm::vec2&  Application::MousePos() const { return pMousePos; }
  const float       Application::AspectRatio() const { return (float)pWindowSize.x / (float)pWindowSize.y; }

  glm::vec2 Application::MouseWheel() const { return pMouseWheel; }
  void      Application::ResetMouseWheel() { pMouseWheel = {.0f, .0f}; }
  bool      Application::MouseFocus() const { return pHasMouseFocus; }

  const Button& Application::Mouse(Gates::Mouse button) const { return pMouseButtons[(uint8_t)button]; }
  const Button& Application::Key(Gates::Key key) const { return pKeyboardKeys[(uint16_t)key]; }

  float    Application::et() const { return pElapsedTime; }
  uint32_t Application::fps() const { return pFrameRate; }

  rcode Application::pOnUpdate() { return rcode::ok; }
  rcode Application::pOnLaunch() { return rcode::ok; }
  rcode Application::pOnImGuiRender() { return rcode::ok; }
  rcode Application::pOnRender() { return rcode::ok; }
  rcode Application::pOnClose() { return rcode::ok; }

  void Application::pHandleFrameBufferResize(GLFWwindow* window, int width, int height) {
    Application* app_instance      = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));
    app_instance->pFrameBufferSize = glm::uvec2 {width, height};
    glViewport(0, 0, width, height);
  }

  void Application::pHandleWindowResize(GLFWwindow* window, int width, int height) {
    Application* app_instance = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));
    app_instance->pWindowSize = glm::uvec2 {width, height};
  }

  void Application::pHandleWindowMove(GLFWwindow* window, int posx, int posy) {
    Application* app_instance = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));
    app_instance->pWindowPos  = glm::uvec2 {posx, posy};
  }

  void Application::pHandleMouseMove(GLFWwindow* window, double posx, double posy) {
    Application* app_instance = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));
    auto         pos          = glm::vec2(posx, posy);

    if (!((pos.x > app_instance->pWindowSize.x) || (pos.y > app_instance->pWindowSize.y))) {
      app_instance->pMousePos =
          (pos / (glm::vec2)app_instance->pWindowSize) * glm::vec2(2.f, 2.f) - glm::vec2(1.f, 1.f);
    }
  }

  void Application::pHandleMouseButton(GLFWwindow* window, int button, int action, int mods) {
    Application* app_instance = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));

    switch (action) {
      case GLFW_RELEASE:
        app_instance->pMouseButtonsNew[button] = false;
        break;

      case GLFW_PRESS:
        app_instance->pMouseButtonsNew[button] = true;
        break;

      default:
        break;
    }
  }

  void Application::pHandleMouseScroll(GLFWwindow* window, double deltax, double deltay) {
    Application* app_instance = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));
    app_instance->pMouseWheel += glm::vec2(deltax, deltay);
  }

  void Application::pHandleMouseEnter(GLFWwindow* window, int enter) {
    Application* app_instance    = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));
    app_instance->pHasMouseFocus = enter;
  }

  void Application::pHandleKeyboardKey(GLFWwindow* window, int key, int scancode, int action, int mods) {
    Application* app_instance = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));
    if (key == -1) key = 0;

    switch (action) {
      case GLFW_RELEASE:
        app_instance->pKeyboardKeysNew[key] = false;
        break;

      case GLFW_PRESS:
        app_instance->pKeyboardKeysNew[key] = true;
        break;

      default:
        break;
    }
  }

  void Application::pPlatformThread() {
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) Logger::Die("GLFW initialization failed");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    pWindow = glfwCreateWindow(pWindowSize.x, pWindowSize.y, "Gates", NULL, NULL);
    if (!pWindow) Logger::Die("GLFW window creation failed");

    glfwSetFramebufferSizeCallback(pWindow, pHandleFrameBufferResize);
    glfwSetWindowSizeCallback(pWindow, pHandleWindowResize);
    glfwSetWindowPosCallback(pWindow, pHandleWindowMove);
    glfwSetCursorPosCallback(pWindow, pHandleMouseMove);
    glfwSetMouseButtonCallback(pWindow, pHandleMouseButton);
    glfwSetScrollCallback(pWindow, pHandleMouseScroll);
    glfwSetCursorEnterCallback(pWindow, pHandleMouseEnter);
    glfwSetKeyCallback(pWindow, pHandleKeyboardKey);

    glfwSetWindowPos(pWindow, pWindowPos.x, pWindowPos.y);
    glfwSetWindowUserPointer(pWindow, (void*)this);

    std::thread thread(&Application::pGraphicsThread, this);
    thread.detach();

    while (!pHasBeenClosed) {
      if (glfwWindowShouldClose(pWindow)) pWantsToClose = true;
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    glfwDestroyWindow(pWindow);
    glfwTerminate();
  }

  void Application::pGraphicsThread() {
    glfwMakeContextCurrent(pWindow);
    glfwSwapInterval(false);

    glewExperimental = GL_TRUE;
    if (!!glewInit()) Logger::Die("GLEW initialization failed");

    std::cout << "[INFO] Using " << glGetString(GL_RENDERER) << " driver with opengl " << glGetString(GL_VERSION)
              << "\n";

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(pWindow, true);
    ImGui_ImplOpenGL3_Init(NULL);

    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    pClock1 = std::chrono::system_clock::now();
    pClock2 = std::chrono::system_clock::now();

    pThreadRunning = pOnLaunch() == rcode::ok ? true : false;

    while (pThreadRunning) {
      while (pThreadRunning) {
        pClock2       = std::chrono::system_clock::now();
        pElapsedTimer = pClock2 - pClock1;
        pClock1       = pClock2;
        pElapsedTime  = pElapsedTimer.count();

        pFrameTimer += pElapsedTime;
        pFrameCount++;

        if (pFrameTimer >= 1.0f) {
          pFrameRate = pFrameCount;
          pFrameTimer -= 1.0f;

          pWindowTittle = pWindowName + " - FPS: " + std::to_string(pFrameRate);
          glfwSetWindowTitle(pWindow, pWindowTittle.c_str());

          pFrameCount = 0;
        }

        for (uint32_t i = 0; i < 8; i++) {
          pMouseButtons[i].pressed  = false;
          pMouseButtons[i].released = false;

          if (pMouseButtonsNew[i] != pMouseButtonsOld[i]) {
            if (pMouseButtonsNew[i]) {
              pMouseButtons[i].pressed = !pMouseButtons[i].held;
              pMouseButtons[i].held    = true;

            } else {
              pMouseButtons[i].released = true;
              pMouseButtons[i].held     = false;
            }
          }

          pMouseButtonsOld[i] = pMouseButtonsNew[i];
        }

        for (uint32_t i = 0; i < 512; i++) {
          pKeyboardKeys[i].pressed  = false;
          pKeyboardKeys[i].released = false;

          if (pKeyboardKeysNew[i] != pKeyboardKeysOld[i]) {
            if (pKeyboardKeysNew[i]) {
              pKeyboardKeys[i].pressed = !pKeyboardKeys[i].held;
              pKeyboardKeys[i].held    = true;

            } else {
              pKeyboardKeys[i].released = true;
              pKeyboardKeys[i].held     = false;
            }
          }

          pKeyboardKeysOld[i] = pKeyboardKeysNew[i];
        }

        glfwPollEvents();

        if (pOnUpdate() != rcode::ok) pThreadRunning = false;

        glViewport(0, 0, pFrameBufferSize.x, pFrameBufferSize.y);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (pOnImGuiRender() != rcode::ok) pThreadRunning = false;

        ImGui::Render();

        glClearColor(
            pClearColor.x * pClearColor.w, pClearColor.y * pClearColor.w, pClearColor.z * pClearColor.w, pClearColor.w);
        glClear(GL_COLOR_BUFFER_BIT);

        if (pOnRender() != rcode::ok) pThreadRunning = false;

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        ImGui::GetDrawData();

        glfwSwapBuffers(pWindow);

        if (pWantsToClose) {
          pThreadRunning = false;
          pWantsToClose  = false;
        }
      }

      if (pOnClose() != rcode::ok) pThreadRunning = true;
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    pHasBeenClosed = true;
  }
}
