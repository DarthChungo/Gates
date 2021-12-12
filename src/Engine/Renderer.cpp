#include "pch.hpp"
#include "Engine/Renderer.hpp"
#include "Engine/Shader.hpp"

namespace SimX {
  static struct {
    GLuint gl_tri_vertex_array  = 0;
    GLuint gl_line_vertex_array = 0;

    GLuint gl_tri_vertex_buffer = 0;
    GLuint gl_tri_index_buffer  = 0;

    GLuint gl_line_vertex_buffer = 0;
    GLuint gl_line_index_buffer  = 0;

    Vertex*   tri_vertex_buffer         = nullptr;
    Vertex*   tri_vertex_buffer_current = nullptr;
    uint32_t* tri_index_buffer          = nullptr;
    uint32_t  tri_vertex_count          = 0;
    uint32_t  tri_index_count           = 0;

    Vertex*   line_vertex_buffer         = nullptr;
    Vertex*   line_vertex_buffer_current = nullptr;
    uint32_t* line_index_buffer          = nullptr;
    uint32_t  line_vertex_count          = 0;
    uint32_t  line_index_count           = 0;

    Renderer::Stats stats = {};

    std::unique_ptr<ShaderProgram> shader_program;

    glm::mat4 view_projection = glm::mat4(1.f);
    glm::mat4 transform       = glm::mat4(1.f);
  } data;

  void Renderer::Init() {
    data.shader_program = std::make_unique<ShaderProgram>("shaders/shader.vert", "shaders/shader.frag");
    data.shader_program->Use();

    data.tri_vertex_buffer = new Vertex[pMaxVertexCount];
    data.tri_index_buffer  = new uint32_t[pMaxIndexCount];

    data.line_vertex_buffer = new Vertex[pMaxVertexCount];
    data.line_index_buffer  = new uint32_t[pMaxIndexCount];

    glCreateVertexArrays(1, &data.gl_tri_vertex_array);
    glBindVertexArray(data.gl_tri_vertex_array);

    glCreateBuffers(1, &data.gl_tri_vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, data.gl_tri_vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, pMaxVertexCount * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);

    glCreateBuffers(1, &data.gl_tri_index_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, data.gl_tri_index_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, pMaxIndexCount * sizeof(uint32_t), nullptr, GL_DYNAMIC_DRAW);

    glEnableVertexArrayAttrib(data.gl_tri_vertex_array, 0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, position));

    glEnableVertexArrayAttrib(data.gl_tri_vertex_array, 1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, color));

    glCreateVertexArrays(1, &data.gl_line_vertex_array);
    glBindVertexArray(data.gl_line_vertex_array);

    glCreateBuffers(1, &data.gl_line_vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, data.gl_line_vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, pMaxVertexCount * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);

    glCreateBuffers(1, &data.gl_line_index_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, data.gl_line_index_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, pMaxIndexCount * sizeof(uint32_t), nullptr, GL_DYNAMIC_DRAW);

    glEnableVertexArrayAttrib(data.gl_line_vertex_array, 0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, position));

    glEnableVertexArrayAttrib(data.gl_line_vertex_array, 1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, color));
  }

  void Renderer::Delete() {
    glDeleteVertexArrays(1, &data.gl_tri_vertex_array);
    glDeleteVertexArrays(1, &data.gl_line_vertex_array);

    glDeleteBuffers(1, &data.gl_tri_vertex_buffer);
    glDeleteBuffers(1, &data.gl_tri_index_buffer);

    glDeleteBuffers(1, &data.gl_line_vertex_buffer);
    glDeleteBuffers(1, &data.gl_line_index_buffer);

    delete[] data.tri_vertex_buffer;
    delete[] data.tri_index_buffer;

    delete[] data.line_vertex_buffer;
    delete[] data.line_index_buffer;
  }

  void BeginTriBatch() { data.tri_vertex_buffer_current = data.tri_vertex_buffer; }

  void BeginLineBatch() { data.line_vertex_buffer_current = data.line_vertex_buffer; }

  void EndTriBatch() {
    data.shader_program->Use();

    glUniformMatrix4fv(glGetUniformLocation(data.shader_program->getProgram(), "u_view_projection"),
                       1,
                       GL_FALSE,
                       &data.view_projection[0][0]);

    glUniformMatrix4fv(
        glGetUniformLocation(data.shader_program->getProgram(), "u_transform"), 1, GL_FALSE, &data.transform[0][0]);

    glBindVertexArray(data.gl_tri_vertex_array);

    glBindBuffer(GL_ARRAY_BUFFER, data.gl_tri_vertex_buffer);
    glBufferSubData(GL_ARRAY_BUFFER, 0, data.tri_vertex_count * sizeof(Vertex), data.tri_vertex_buffer);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, data.gl_tri_index_buffer);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, data.tri_index_count * sizeof(uint32_t), data.tri_index_buffer);
  }

  void EndLineBatch() {
    data.shader_program->Use();

    glUniformMatrix4fv(glGetUniformLocation(data.shader_program->getProgram(), "u_view_projection"),
                       1,
                       GL_FALSE,
                       &data.view_projection[0][0]);

    glUniformMatrix4fv(
        glGetUniformLocation(data.shader_program->getProgram(), "u_transform"), 1, GL_FALSE, &data.transform[0][0]);

    glBindVertexArray(data.gl_line_vertex_array);

    glBindBuffer(GL_ARRAY_BUFFER, data.gl_line_vertex_buffer);
    glBufferSubData(GL_ARRAY_BUFFER, 0, data.line_vertex_count * sizeof(Vertex), data.line_vertex_buffer);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, data.gl_line_index_buffer);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, data.line_index_count * sizeof(uint32_t), data.line_index_buffer);
  }

  void FlushTriBatch() {
    glBindVertexArray(data.gl_tri_vertex_array);

    glBindBuffer(GL_ARRAY_BUFFER, data.gl_tri_vertex_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, data.gl_tri_index_buffer);
    glDrawElements(GL_TRIANGLES, data.tri_index_count, GL_UNSIGNED_INT, nullptr);

    data.stats.tri_index_count += data.tri_index_count;
    data.stats.tri_vertex_count += data.tri_vertex_count;

    data.tri_index_count  = 0;
    data.tri_vertex_count = 0;

    data.stats.draw_calls++;
  }

  void FlushLineBatch() {
    glBindVertexArray(data.gl_line_vertex_array);

    glBindBuffer(GL_ARRAY_BUFFER, data.gl_line_vertex_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, data.gl_line_index_buffer);
    glDrawElements(GL_LINES, data.line_index_count, GL_UNSIGNED_INT, nullptr);

    data.stats.line_index_count += data.line_index_count;
    data.stats.line_vertex_count += data.line_vertex_count;

    data.line_index_count  = 0;
    data.line_vertex_count = 0;

    data.stats.draw_calls++;
  }

  void Renderer::BeginBatch() {
    BeginTriBatch();
    BeginLineBatch();
  }

  void Renderer::EndBatch() {
    EndTriBatch();
    EndLineBatch();
  }

  void Renderer::FlushBatch() {
    FlushTriBatch();
    FlushLineBatch();
  }

  void Renderer::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color) {
    if ((data.tri_index_count + 6) >= pMaxIndexCount || (data.tri_vertex_count + 4) >= pMaxVertexCount) {
      EndTriBatch();
      FlushTriBatch();
      BeginTriBatch();
    }

    data.tri_vertex_buffer_current->position = {position.x, position.y, .0f};
    data.tri_vertex_buffer_current->color    = color;
    data.tri_vertex_buffer_current++;

    data.tri_vertex_buffer_current->position = {position.x + size.x, position.y, .0f};
    data.tri_vertex_buffer_current->color    = color;
    data.tri_vertex_buffer_current++;

    data.tri_vertex_buffer_current->position = {position.x + size.x, position.y + size.y, .0f};
    data.tri_vertex_buffer_current->color    = color;
    data.tri_vertex_buffer_current++;

    data.tri_vertex_buffer_current->position = {position.x, position.y + size.y, .0f};
    data.tri_vertex_buffer_current->color    = color;
    data.tri_vertex_buffer_current++;

    data.tri_index_buffer[data.tri_index_count + 0] = 0 + data.tri_vertex_count;
    data.tri_index_buffer[data.tri_index_count + 1] = 1 + data.tri_vertex_count;
    data.tri_index_buffer[data.tri_index_count + 2] = 2 + data.tri_vertex_count;

    data.tri_index_buffer[data.tri_index_count + 3] = 2 + data.tri_vertex_count;
    data.tri_index_buffer[data.tri_index_count + 4] = 3 + data.tri_vertex_count;
    data.tri_index_buffer[data.tri_index_count + 5] = 0 + data.tri_vertex_count;

    data.tri_vertex_count += 4;
    data.tri_index_count += 6;

    data.stats.quads_drawn++;
  }

  void Renderer::DrawTri(const glm::vec2& v1, const glm::vec2& v2, const glm::vec2& v3, const glm::vec4 color) {
    if ((data.tri_index_count + 3) >= pMaxIndexCount || (data.tri_vertex_count + 3) >= pMaxVertexCount) {
      EndTriBatch();
      FlushTriBatch();
      BeginTriBatch();
    }

    data.tri_vertex_buffer_current->position = {v1.x, v1.y, .0f};
    data.tri_vertex_buffer_current->color    = color;
    data.tri_vertex_buffer_current++;

    data.tri_vertex_buffer_current->position = {v2.x, v2.y, .0f};
    data.tri_vertex_buffer_current->color    = color;
    data.tri_vertex_buffer_current++;

    data.tri_vertex_buffer_current->position = {v3.x, v3.y, .0f};
    data.tri_vertex_buffer_current->color    = color;
    data.tri_vertex_buffer_current++;

    data.tri_index_buffer[data.tri_index_count + 0] = 0 + data.tri_vertex_count;
    data.tri_index_buffer[data.tri_index_count + 1] = 1 + data.tri_vertex_count;
    data.tri_index_buffer[data.tri_index_count + 2] = 2 + data.tri_vertex_count;

    data.tri_vertex_count += 3;
    data.tri_index_count += 3;

    data.stats.tris_drawn++;
  }

  void Renderer::DrawCircle(const glm::vec2& position,
                            const float&     radius,
                            const uint32_t&  segments,
                            const glm::vec4& color) {
    if ((data.tri_index_count + segments * 3) >= pMaxIndexCount ||
        (data.tri_vertex_count + segments + 1) >= pMaxVertexCount) {
      EndTriBatch();
      FlushTriBatch();
      BeginTriBatch();
    }

    float inc = glm::two_pi<float>() / segments;

    data.tri_vertex_buffer_current->position = {position.x, position.y, .0f};
    data.tri_vertex_buffer_current->color    = color;
    data.tri_vertex_buffer_current++;

    for (uint32_t current = 0; current < segments; current++) {
      data.tri_vertex_buffer_current->position = {
          glm::cos(current * inc) * radius + position.x, glm::sin(current * inc) * radius + position.y, .0f};
      data.tri_vertex_buffer_current->color = color;
      data.tri_vertex_buffer_current++;

      data.tri_index_buffer[data.tri_index_count + 0] = data.tri_vertex_count;
      data.tri_index_buffer[data.tri_index_count + 1] = data.tri_vertex_count + current + 1;
      data.tri_index_buffer[data.tri_index_count + 2] =
          data.tri_vertex_count + (current == segments - 1 ? 1 : current + 2);
      data.tri_index_count += 3;
    }

    data.tri_vertex_count += (segments + 1);
    data.stats.circles_drawn++;
  }

  void Renderer::DrawLine(const glm::vec2& pos1, const glm::vec2& pos2, const glm::vec4& color) {
    if ((data.line_index_count + 2) >= pMaxIndexCount || (data.line_vertex_count + 2) >= pMaxVertexCount) {
      EndLineBatch();
      FlushLineBatch();
      BeginLineBatch();
    }

    data.line_vertex_buffer_current->position = {pos1.x, pos1.y, .0f};
    data.line_vertex_buffer_current->color    = color;
    data.line_vertex_buffer_current++;

    data.line_vertex_buffer_current->position = {pos2.x, pos2.y, .0f};
    data.line_vertex_buffer_current->color    = color;
    data.line_vertex_buffer_current++;

    data.line_index_buffer[data.line_index_count + 0] = 0 + data.line_vertex_count;
    data.line_index_buffer[data.line_index_count + 1] = 1 + data.line_vertex_count;

    data.line_vertex_count += 2;
    data.line_index_count += 2;

    data.stats.lines_drawn++;
  }

  void Renderer::SetViewProjection(const glm::mat4& view_projection) { data.view_projection = view_projection; }

  void Renderer::SetTransform(const glm::vec3& transform) {
    data.transform = glm::translate(glm::mat4(1.f), transform);
  }

  void Renderer::UseCamera(const OrthographicCamera& camera) {
    data.view_projection = camera.getViewProjectionMatrix();
  }

  void                   Renderer::ResetStats() { memset(&data.stats, 0, sizeof(Stats)); }
  const Renderer::Stats& Renderer::GetStats() { return data.stats; }
}
