#version 330 core
layout (location = 0) in vec3 aPos;

void main() {
    // Most of the time normalization of vectors to screen space is also done here
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}