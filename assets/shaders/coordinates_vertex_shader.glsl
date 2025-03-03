#version 330 core
layout (location = 0) in vec3 aPos; // the position of the vertex attribute
layout (location = 1) in vec2 aTexCoord; // this takes in the tex coords from the vertices array

out vec3 outColor; // the color to be passed to the fragment shader
out vec2 texCoord; // this passes the tex coords to the fragment shader so color from the texture is displayed

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main()
{
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(aPos, 1.0); // declaring the position to match the vertices
    texCoord = aTexCoord; // this will copy the texture coords from the vertices array into the shader variable so it can be sent to the fragment shader
}