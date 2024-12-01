#include "Shader.h"
#include <iostream>
#include <glad/glad.h>
#include <glfw3.h>
#include <fstream>
#include <sstream>
#include <fwd.hpp>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>

std::string Shader::LoadShader(const char* shaderPath)
{
    std::string shaderCode;
    std::ifstream shaderFile;
    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        shaderFile.open(shaderPath);

        std::stringstream shaderStream;
        shaderStream << shaderFile.rdbuf();

        shaderFile.close();
        shaderCode = shaderStream.str();

        return shaderCode;
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "Could not load shader file from path - " << shaderPath << "\n";
        return "";
    }
}

unsigned int Shader::LoadVertexShader(const char* vertexShaderPath)
{
    int result;
    char log[512];

    std::string shaderCodeString = LoadShader(vertexShaderPath);
    const char* shaderCode = shaderCodeString.c_str();

    unsigned int shaderObject;

    shaderObject = glCreateShader(GL_VERTEX_SHADER);

    if (!result)
    {
        glGetShaderInfoLog(shaderObject, 512, NULL, log);
        std::cout << "Failed to compile vertex shader \n" << log << std::endl;
    }

    return shaderObject;
}

unsigned int Shader::LoadFragmentShader(const char* fragmentShaderPath)
{
    int result;
    char log[512];

    std::string shaderCodeString = LoadShader(fragmentShaderPath);
    const char* shaderCode = shaderCodeString.c_str();

    unsigned int shaderObject;

    shaderObject = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(shaderObject, 1, &shaderCode, NULL);
    glCompileShader(shaderObject);

    glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &result);

    if (!result)
    {
        glGetShaderInfoLog(shaderObject, 512, NULL, log);
        std::cout << "Failed to compile fragment shader \n" << Log << std::endl;
    }

    return shaderObject;
}

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
    if (!RecompileShader(vertexPath, fragmentPath))
    {
        delete this;
    }
}

bool Shader::RecompileShader(const char* vertexPath, const char* fragmentPath)
{
    int result;
    char log[512];

    unsigned int VertexShader = LoadVertexShader(vertexPath);
    unsigned int FragmentShader = LoadFragmentShader(fragmentPath);

    myShaderProgram = glCreateProgram();
    glAttachShader(myShaderProgram, VertexShader);
    glAttachShader(myShaderProgram, FragmentShader);
    glLinkProgram(myShaderProgram);

    glGetProgramiv(myShaderProgram, GL_LINK_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(myShaderProgram, 512, NULL, log);
        std::cout << "Shader program could not compile\n" << log << std::endl;

        glDeleteShader(VertexShader);
        glDeleteShader(FragmentShader);

        myShaderProgram = 0;

        return false;
    }

    glDeleteShader(VertexShader);
    glDeleteShader(FragmentShader);

    myShaderProgram = 0;

    return false;
}

void Shader::SetMatrix4(glm::mat4 matrix, const std::string& name)
{
    glUniformMatrix4fv(glGetUniformLocation(myShaderProgram, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::SetVector4(glm::vec4 vector4Value, const std::string& name)
{
    glUniform4f(glGetUniformLocation(myShaderProgram, name.c_str()), vector4Value.x, vector4Value.y, vector4Value.z, vector4Value.w);
}

void Shader::SetFloat(float floatValue, const std::string& name)
{
    glUniform1f(glGetUniformLocation(myShaderProgram, name.c_str()), floatValue);
}