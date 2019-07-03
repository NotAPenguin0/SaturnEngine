#version 430 core

void main() {
    // empty shader
    gl_FragDepth = gl_FragCoord.z;
}