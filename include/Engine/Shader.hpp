#ifndef SIMX_SHADER_HPP
#define SIMX_SHADER_HPP

#include "pch.hpp"

namespace SimX {
  class ShaderProgram {
   public:
    ShaderProgram(const char* vertex_path, const char* fragment_path);

    GLuint getProgram();
    void   Use();

   private:
    GLuint pProgram;
  };
}

#endif
