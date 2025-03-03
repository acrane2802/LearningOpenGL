#version 330 core
out vec4 FragColor; // the final output to screen

uniform vec4 outColor; // the input uniform we set

void main()
{
    FragColor = outColor; // setting the final color to the input color
}