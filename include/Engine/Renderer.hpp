#ifndef SIMX_RENDERER_HPP
#define SIMX_RENDERER_HPP

#include "pch.hpp"
#include "Engine/OrthographicCamera.hpp"

namespace Gates {
  struct Vertex {
    glm::vec3 position;
    glm::vec4 color;
  };

  class Renderer {
   public:
    static void Init();
    static void Delete();

    static void BeginBatch();
    static void EndBatch();
    static void FlushBatch();

    static void DrawQuad(const glm::vec2& position,
                         const glm::vec2& size,
                         const glm::vec4& color = {1.f, 1.f, 1.f, 1.f});

    static void DrawTri(const glm::vec2& v1,
                        const glm::vec2& v2,
                        const glm::vec2& v3,
                        const glm::vec4  color = {1.f, 1.f, 1.f, 1.f});

    static void DrawCircle(const glm::vec2& position,
                           const float&     radius,
                           const uint32_t&  segments,
                           const glm::vec4& color = {1.f, 1.f, 1.f, 1.f});

    static void DrawLine(const glm::vec2& pos1, const glm::vec2& pos2, const glm::vec4& color = {1.f, 1.f, 1.f, 1.f});

    static void SetViewProjection(const glm::mat4& view_projection);
    static void SetTransform(const glm::vec3& transform);
    static void UseCamera(const OrthographicCamera& camera);

    struct Stats {
      uint32_t quads_drawn       = 0;
      uint32_t draw_calls        = 0;
      uint32_t tris_drawn        = 0;
      uint32_t lines_drawn       = 0;
      uint32_t circles_drawn     = 0;
      uint32_t tri_vertex_count  = 0;
      uint32_t tri_index_count   = 0;
      uint32_t line_vertex_count = 0;
      uint32_t line_index_count  = 0;
    };

    static void         ResetStats();
    static const Stats& GetStats();

   public:
    static constexpr uint32_t pMaxVertexCount = 20000;
    static constexpr uint32_t pMaxIndexCount  = 30000;
  };
}

#endif
