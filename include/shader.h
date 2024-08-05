#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
public:
    unsigned int ID;

    Shader(const char* vertexPath, const char* fragmentPath) {
        std::string vertexSource;
        std::string fragmentSource;
        std::ifstream vertexShaderFile;
        std::ifstream fragmentShaderFile;

        vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        try {
            vertexShaderFile.open(vertexPath);
            fragmentShaderFile.open(fragmentPath);
            std::stringstream vertexShaderStream, fragmentShaderStream;

            vertexShaderStream << vertexShaderFile.rdbuf();
            fragmentShaderStream << fragmentShaderFile.rdbuf();

            vertexShaderFile.close();
            fragmentShaderFile.close();

            vertexSource = vertexShaderStream.str();
            fragmentSource = fragmentShaderStream.str();
        } catch(std::ifstream::failure e) {
            std::cout << "Shader file not read successfully" << std::endl;
        }

        const char* vertexShaderSource = vertexSource.c_str();
        const char* fragmentShaderSource = fragmentSource.c_str();
    }

    void use();

    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
};

#endif