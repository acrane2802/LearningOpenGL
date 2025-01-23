#include "Shader.h"

Shader::Shader(const char *vertexPath, const char *fragmentPath)
{
    // open the files at the paths specified
    std::ifstream vertexShaderFile;
    std::ifstream fragmentShaderFile;
    vertexShaderFile.open(vertexPath);
    fragmentShaderFile.open(fragmentPath);

    // begin reading into data into buffers
    std::stringstream vertexBuffer;
    std::stringstream fragmentBuffer;
    vertexBuffer << vertexShaderFile.rdbuf();
    fragmentBuffer << fragmentShaderFile.rdbuf();

    // close the file once it is done
    vertexShaderFile.close();
    fragmentShaderFile.close();

    // convert those data buffers into strings
    std::string vertexShaderSource = vertexBuffer.str();
    std::string fragmentShaderSource = fragmentBuffer.str();

    // retrieve the data from the strings into an opengl compatible format
    const char* vertexShaderData = vertexShaderSource.c_str();
    const char* fragmentShaderData = fragmentShaderSource.c_str();

    // create the shaders in opengl
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    // associate the shader id with the shader data
    glShaderSource(vertexShader, 1, &vertexShaderData, nullptr);
    glShaderSource(fragmentShader, 1, &fragmentShaderData, nullptr);

    // compile both shaders
    glCompileShader(vertexShader);
    glCompileShader(fragmentShader);

    // check that the shaders compiled successfully
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    // if the compilation fails, it retrieves the error, stores it in the array, then prints it out
    if(!success)
    {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cout << "Vertex Shader failed to compile!\n" << infoLog << std::endl;
    }

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    // if the compilation fails, it retrieves the error, stores it in the array, then prints it out
    if(!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cout << "Fragment Shader failed to compile!\n" << infoLog << std::endl;
    }


    // create a shader program at the id specified, then add the compiled shaders and link
    ID = glCreateProgram();
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    glLinkProgram(ID);

    // check the program status, then print an error if it fails
    glGetProgramiv(ID, GL_LINK_STATUS, &success);

    if(!success)
    {
        glGetProgramInfoLog(ID, 512, nullptr, infoLog);
        std::cout << "Shader Program failed to link!\n" << infoLog << std::endl;
    }

    // clean up the compiled shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

// tell opengl to use the given program
void Shader::use()
{
    glUseProgram(ID);
}

// set the shader uniforms that are available
void Shader::setBool(const std::string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), static_cast<int>(value));
}

void Shader::setInt(const std::string &name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

