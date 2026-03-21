#version 330

layout (location = 0) in vec3 vertex;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

//Zmienne jednorodne
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;

uniform vec3 lightPos;

out vec2 iTexCoord;
out vec4 iNormal;
out vec4 ic;
out vec4 l;
out vec4 v;

void main(void) {
    vec4 vertex4 =  vec4(vertex,1.0);
    
    vec4 lp = vec4(lightPos,1.0);
    l = normalize(V * lp - V*M*vertex4); //wektor do œwiat³a w przestrzeni oka
    v = normalize(vec4(0, 0, 0, 1) - V * M * vertex4); //wektor do obserwatora w przestrzeni oka
    iNormal = normalize(V * M * vec4(normal,0.0)); //wektor normalny w przestrzeni oka

    iTexCoord = texCoord;
    gl_Position=P*V*M*vertex4;
}
