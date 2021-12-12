#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec4 color;

out vec4 our_color;

uniform mat4 u_view_projection;
uniform mat4 u_transform;

void main() {
    gl_Position = u_view_projection * u_transform * vec4(position, 1.0f);
    our_color = color;
}
