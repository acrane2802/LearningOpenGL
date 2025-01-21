#version 330 core // the OpenGL version
out vec4 FragColor; // the final output to screen

uniform vec4 ourColor; // the input uniform we set

void main()
{
    FragColor = ourColor; // setting the final color to the input color
}