#version 330 core // the OpenGL version
layout (location = 0) in vec3 aPos; // the position of the vertex attribute
layout (location = 1) in vec3 aColor; // this is the start of the next group of vec3 data that holds the color in the array
layout (location = 2) in vec2 aTexCoord; // this takes in the tex coords from the vertices array

out vec3 outColor; // the color to be passed to the fragment shader
out vec2 texCoord; // this passes the tex coords to the fragment shader so color from the texture is displayed

uniform mat4 transform;

void main()
{
    gl_Position = transform * vec4(aPos, 1.0); // declaring the position to match the vertices
    outColor = aColor; // this assigns a shader variable to the color retrieve from the vertices
    texCoord = aTexCoord; // this will copy the texture coords from the vertices array into the shader variable so it can be sent to the fragment shader
}