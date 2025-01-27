#ifndef SHADER_H
#define SHADER_H

#include <glad/gl.h>

#include <sstream>
#include <fstream>
#include <string>
#include <iostream>

class Shader
{
public:

    // shader program ID and shader file IDs
    unsigned int ID;
    unsigned int vertexShader, fragmentShader;

    // shader constructor to build the program
    Shader(const char* vertexPath, const char* fragmentPath);

    // custom method to call glUseProgram
    void use();

    // functions to use the shader uniforms
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
};
#endif