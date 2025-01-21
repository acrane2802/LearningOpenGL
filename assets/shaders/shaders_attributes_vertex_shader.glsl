#version 330 core // the OpenGL version
layout (location = 0) in vec3 aPos; // the position of the vertex attribute
layout (location = 1) in vec3 aColor; // this is the start of the next group of vec3 data that holds the color in the array

out vec3 ourColor; // the color to be passed to the fragment shader

void main()
{
    gl_Position = vec4(aPos, 1.0); // declaring the position to match the vertices
    ourColor = aColor; // this assigns a shader variable to the color retrieve from the vertices
}