#version 330 core // the OpenGL version
out vec4 FragColor; // the final output to screen
in vec3 outPos; // this is the color taken from the vertices in code, then saved in the shader variable

void main()
{
    FragColor = vec4(outPos, 1.0f); // setting the final color to the input color
}