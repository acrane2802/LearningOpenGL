#version 330 core
out vec4 FragColor; // the final output to screen
in vec3 outColor; // this is the color taken from the vertices in code, then saved in the shader variable
in vec2 texCoord; // this reads the coords from the vertex shader

uniform sampler2D textureColor1;
uniform sampler2D textureColor2;

void main()
{
    // this samples the texture color from the data at every single point of the shader's process using the color from the texture and the coords
    // texture(texture data added via uniform, texture coordinates from the vertex shader)
    // this mixes the two textures, the third argument mixes it 80% 1, 20% 2
    FragColor = mix(texture(textureColor1, texCoord), texture(textureColor2, texCoord), 0.2);
}