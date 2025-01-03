#version 450
layout(location = 0) in vec4 in_vec4_frag_color;
layout (location = 0) out vec4 out_color;

void main() {
    out_color = in_vec4_frag_color;
}