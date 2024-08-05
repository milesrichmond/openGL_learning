#include "include/glad/glad.h"
#include <GLFW/glfw3.h>
#include "include/shader.h"
#include <iostream>
#include <cmath>

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n" 
    "layout (location = 1) in vec3 aColor;\n"
    "out vec3 color;\n"
    "void main() {\n" 
    "gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "color = aColor;\n"
    "}\0";
const char *fragmentShaderSource = "#version 330 core\n" 
    "out vec4 FragColor;\n" 
    "in vec3 color;\n" 
    "void main() {\n"
    "FragColor = vec4(color, 1.0);\n"
    "}\0";

// Simple user input
void processInput(GLFWwindow *window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

// Handles user resizing of window
// Called by GLFW whenever window size changes
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

int main() {
    // Initialization
    // GLFW init
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // GLFW window declaration & configuration
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if(window == NULL) {
        std::cout << "failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // GLAD init
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Viewport
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // Configures callback for window resizes

    // Shader build & compilation
        // Vertex Shader
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); // 2nd arg is how many strings are being passed
    glCompileShader(vertexShader);

        // Fragment Shader
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // Linking shaders into shader program
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Checking for compile time errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "Vertex shader compilation failed\n" << infoLog << std::endl;
    }
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "Fragment shader compilation failed\n" << infoLog << std::endl;
    }
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "Shader program linking failed\n" << infoLog << std::endl;
    }

    glUseProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Rectangle vertices (EBOs will reduce the repeated vertices)
    float vertices[] = {
        0.5f,  0.5f, 0.0f, /**/ 1.0f, 0.0f, 0.0f, // top right
        0.5f, -0.5f, 0.0f, /**/ 0.0f, 1.0f, 0.0f, // bottom right
       -0.5f, -0.5f, 0.0f, /**/ 1.0f, 1.0f, 1.0f, // bottom left
       -0.5f,  0.5f, 0.0f, /**/ 0.0f, 0.0f, 1.0f // top left
    };
    unsigned int indices[] = { // note that we start from 0!
        0, 1, 3, // first triangle
        1, 2, 3 // second triangle
    };

    // Buffer & Array Object setup
    // Vertex Buffer Object, reduces the traffic from CPU to GPU
    // Vertex Array Object, stores the config below to simplify future calls
    // Element Buffer Object, allows indexed drawing (tells OpenGL what order to render in in order to reduce the number of vertices stored)
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO); // sets VBO to the ID for the buffer
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO); // Bound first before VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // Future calls targeting the GL_ARRAY_BUFFER will target the currently bound buffer (VBO in this case)

    // The 4th argument specifies how the GPU should handle the data, 3 options:    GL_STREAM_DRAW  : sent once, used a few times
    //                                                                              GL_STATIC_DRAW  : sent once, used lots
    //                                                                              GL_DYNAMIC_DRAW : data is changing, used lots
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // Copies the vertex data into buffer memory

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Arg explanation: 1: which vertex attribute to configure (with these simple shaders, the aPos vertex attribute was set to location 0, hence the passed argument)
    //                  2: size of the vertex attribute. vec3 has 3 values
    //                  3: data type
    //                  4: should the data be normalized? (not relavant in our situation, thus set to GL_FALSE)
    //                  5: stride (the space between vertex attributes.) Each set of position data is 3 floats away (xyzxyzxyz)
    //                  6: position data offset from the beginning of the buffer. (icky cast to void*)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0); // vertex attributes are disabled by default. This function is passed the attribute location

    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Wireframe mode
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Render Loop
    while(!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(0.2f,0.3f,0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT); // Clears the buffer for the next frame and prevents previous results from being displayed

        glUseProgram(shaderProgram);

        glBindVertexArray(VAO); // indirectly binds the VBO
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}