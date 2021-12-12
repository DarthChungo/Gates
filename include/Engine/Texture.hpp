#ifndef GATES_TEXTURE_HPP
#define GATES_TEXTURE_HPP

#include "pch.hpp"

namespace Gates {
  class Texture {
   public:
    void Load(const std::string& filepath);
    void Load(const glm::vec4& color);

    void Release();

    GLuint getId() const;

   private:
    GLuint pId = 0;
  };
}

#endif
