#version 330

layout (location = 0) in vec3 vertex;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

//Zmienne jednorodne
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;


out vec2 iTexCoord;

void main(void) {
    iTexCoord = texCoord;
    gl_Position=P*V*M*vec4(vertex,1.0f);
}
