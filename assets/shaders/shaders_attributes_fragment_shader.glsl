#version 330 core // the OpenGL version
out vec4 FragColor; // the final output to screen
in vec3 ourColor; // this is the color taken from the vertices in code, then saved in the shader variable

void main()
{
    FragColor = vec4(ourColor, 1.0f); // setting the final color to the input color
}