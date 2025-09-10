#version 330 core // the OpenGL version
out vec4 FragColor; // the final output to screen

uniform vec3 objectColor;
uniform vec3 lightColor;

void main()
{
    FragColor = vec4(lightColor * objectColor, 1.0f);
}