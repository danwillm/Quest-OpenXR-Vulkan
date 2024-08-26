#version 450

layout (location = 0) out vec3 vec3_frag_color;

vec2 vec2_positions[3] = vec2[](
vec2(0.0, -0.5),
vec2(0.5, 0.5),
vec2(-0.5, 0.5)
);

vec3 vec3_colors[3] = vec3[](
vec3(1.0, 0.0, 0.0),
vec3(0.0, 1.0, 0.0),
vec3(0.0, 0.0, 1.0)
);

void main() {
    gl_Position = vec4(vec2_positions[gl_VertexIndex], 0.0, 1.0);
    vec3_frag_color = vec3_colors[gl_VertexIndex];
}