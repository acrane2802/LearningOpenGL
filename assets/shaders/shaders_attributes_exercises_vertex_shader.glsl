#version 330 core
layout (location = 0) in vec3 aPos; // the position of the vertex attribute
layout (location = 1) in vec3 aColor; // this is the start of the next group of vec3 data that holds the color in the array

uniform float horizontalOffset;

out vec3 outColor; // the color to be passed to the fragment shader
out vec3 outPos;

void main()
{
    gl_Position = vec4(aPos.x + horizontalOffset, (aPos.y * -1.0), aPos.z, 1.0); // declaring the position to match the vertices
    outColor = aColor; // this assigns a shader variable to the color retrieve from the vertices
    outPos = aPos;
}