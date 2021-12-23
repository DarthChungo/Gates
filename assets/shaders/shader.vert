#version 460 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec4 color;
layout (location = 2) in vec2 tex_coord;
layout (location = 3) in float tex_index;

out vec4 our_color;
out vec2 our_tex_coord;
out float our_tex_index;

uniform mat4 u_view_projection;
uniform mat4 u_transform;

void main() {
    gl_Position = u_view_projection * u_transform * vec4(position, 1.0f);
    our_color = color;
    our_tex_coord = tex_coord;
    our_tex_index = tex_index;
}
