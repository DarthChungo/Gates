#version 330 core

in vec4 our_color;
in vec2 our_tex_coord;
in float our_tex_index;

out vec4 color;

uniform sampler2D u_textures[8];

void main() {
  int index = int(our_tex_index);
  color = texture(u_textures[index], our_tex_coord) * our_color;
}
