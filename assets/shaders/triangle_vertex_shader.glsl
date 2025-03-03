#version 330 core
layout (location = 0) in vec3 aPos; // the position of the vertex attribute

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0); // specify the location of the vertices
}