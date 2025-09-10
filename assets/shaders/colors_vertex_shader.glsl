#version 330 core // the OpenGL version
layout (location = 0) in vec3 aPos; // the position of the vertex attribute

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main()
{
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(aPos, 1.0); // declaring the position to match the vertices
}