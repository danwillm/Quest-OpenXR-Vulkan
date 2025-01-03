#version 450

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec4 inColor;

layout (location = 0) out vec4 fragColor;

layout (binding = 0) uniform MVP {
    mat4 mat;
} mvp;

void main() {
    gl_Position = mvp.mat * vec4(inPosition, 1.0);
    fragColor = inColor;
}