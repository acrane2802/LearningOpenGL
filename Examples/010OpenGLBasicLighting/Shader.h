#ifndef SHADER_H
#define SHADER_H

#include <glad/gl.h>

#include <sstream>
#include <fstream>
#include <string>
#include <iostream>

#include <glm/glm.hpp>

class Shader {
public:

    // shader program ID and shader file IDs
    unsigned int ID;
    unsigned int vertexShader, fragmentShader;

    // shader constructor to build the program
    Shader(const char* vertexPath, const char* fragmentPath);

    // custom method to call glUseProgram
    void use() const;

    // functions to use the shader uniforms
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;

    void setVec2(const std::string &name, float x, float y) const;
    void setVec2(const std::string &name, glm::vec2 &vec) const;
    void setVec3(const std::string &name, float x, float y, float z) const;
    void setVec3(const std::string &name, glm::vec3 &vec) const;
    void setVec4(const std::string &name, float x, float y, float z, float w) const;
    void setVec4(const std::string &name, glm::vec4 &vec) const;

    void setMat2(const std::string &name, const glm::mat2 &mat) const;
    void setMat3(const std::string &name, const glm::mat3 &mat) const;
    void setMat4(const std::string &name, const glm::mat4 &mat) const;
};
#endif