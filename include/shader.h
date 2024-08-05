#ifndef SHADER_H
#define SHADER_H

#include "include/glad/glad.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
public:
    unsigned int ID;

    Shader(const char* vertexPath, const char* fragmentPath) {
        // reading files
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

        // Compilation
        unsigned int vertex, fragment;
        int success;
        char infoLog[512];

        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vertexShaderSource, NULL);
        glCompileShader(vertex);
        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
        if(!success) {
            glGetShaderInfoLog(vertex, 512, NULL, infoLog);
            std::cout << "Vertex shader compilation failed\n" << infoLog << std::endl;
        }

        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fragmentShaderSource, NULL);

    }

    void use();

    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
};

#endif