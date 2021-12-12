#include "pch.hpp"
#include "Engine/Shader.hpp"
#include "Util/Logger.hpp"

namespace SimX {
  ShaderProgram::ShaderProgram(const char* vertex_path, const char* fragment_path) {
    std::string       vertex_code, fragment_code;
    std::ifstream     vertex_file, fragment_file;
    std::stringstream vertex_stream, fragment_stream;

    vertex_file.exceptions(std::ifstream::badbit);
    fragment_file.exceptions(std::ifstream::badbit);

    vertex_file.open(vertex_path);
    fragment_file.open(fragment_path);

    if (!vertex_file.good()) Logger::Die("Cannot read vertex shader file");
    if (!fragment_file.good()) Logger::Die("Cannot read fragment shader file");

    vertex_stream << vertex_file.rdbuf();
    fragment_stream << fragment_file.rdbuf();

    vertex_file.close();
    fragment_file.close();

    vertex_code   = vertex_stream.str();
    fragment_code = fragment_stream.str();

    const GLchar* vertex_gl_code   = vertex_code.c_str();
    const GLchar* fragment_gl_code = fragment_code.c_str();

    GLuint vertex, fragment;
    GLint  success;
    GLchar infoLog[512];

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertex_gl_code, NULL);
    glCompileShader(vertex);

    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(vertex, 512, NULL, infoLog);
      Logger::Die("Vertex shader compilation failed with message:\n\t" + std::string(infoLog));
    }

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragment_gl_code, NULL);
    glCompileShader(fragment);

    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(fragment, 512, NULL, infoLog);
      Logger::Die("Fragment shader compilation failed with message:\n\t" + std::string(infoLog));
    }

    pProgram = glCreateProgram();
    glAttachShader(pProgram, vertex);
    glAttachShader(pProgram, fragment);
    glLinkProgram(pProgram);

    glGetProgramiv(pProgram, GL_LINK_STATUS, &success);
    if (!success) {
      glGetProgramInfoLog(pProgram, 512, NULL, infoLog);
      Logger::Die("Shader program linking failed with message:\n\t" + std::string(infoLog));
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
  }

  GLuint ShaderProgram::getProgram() { return pProgram; }
  void   ShaderProgram::Use() { glUseProgram(pProgram); }
}
