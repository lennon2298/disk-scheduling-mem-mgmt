#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#include <kazmath/kazmath.h>

#include "Resources/Headers/stb_image.h"
#include "rendertext.h"
#include "disksch.h"

//Vertex BG Shader
const char *vertexBgShaderSource ="#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec2 aTex;\n"
    "out vec3 ourColor;\n"
    "out vec2 bgCoords;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos, 1.0);\n"
    "   bgCoords = aTex;\n"
    "}\0";

//Fragment Shader (background)
const char *fragmentBgShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec3 ourColor;\n"
    "in vec2 bgCoords;\n"
    "uniform sampler2D bgTex;\n"
    "uniform sampler2D optTex;\n"
    
    "void main()\n"
    "{\n"
    "   FragColor = texture(bgTex, bgCoords);\n"
    "}\n\0";

int shaderProgram;
unsigned int VBO[20], VAO[20], EBO;
unsigned int texture[20];

int vertexShader, fragmentShader;
int success;
char infoLog[512];

void compileShader()
{
    // vertex shader
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexBgShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    success;
    infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        printf ("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n");
        printf ("%s\n", infoLog);

    }
    // fragment shader
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentBgShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        printf ("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n");
        printf ("%s\n", infoLog);
    }
}

void initBg()
{
    
    // link shaders
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        printf( "ERROR::SHADER::PROGRAM::LINKING_FAILED\n");
        printf ("%s\n", infoLog);
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    //Generating Textures
    
    int width, height, nrChannels;
    unsigned char *data;

    //Texture 1
    glGenTextures(1, &texture[0]);

    glBindTexture(GL_TEXTURE_2D, texture[0]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(GL_TRUE);
    #ifdef __linux__
    data = stbi_load("../Source/Resources/Textures/background.png", &width, &height, &nrChannels, 0);
    #elif _WIN32
    data = stbi_load("../../Source/Resources/Textures/background.png", &width, &height, &nrChannels, 0);
    #endif
    if(data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        printf("Failed to load Texture\n");
    }
    stbi_image_free(data);

    float bg[] = {
         1.0f,  1.0f, 0.0f,  1.0f,  1.0f,
         1.0f, -1.0f, 0.0f,  1.0f,  0.0f,
        -1.0f, -1.0f, 0.0f,  0.0f,  0.0f,
        -1.0f,  1.0f, 0.0f,  0.0f,  1.0f
    };

    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 2,  // first Triangle
        0, 2, 3   // second Triangle
    };

    glGenVertexArrays(1, &VAO[0]);
    glGenBuffers(1, &VBO[0]);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO[0]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(bg), bg, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //Texture 2
    glGenTextures(1, &texture[1]);

    glBindTexture(GL_TEXTURE_2D, texture[1]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(GL_TRUE);
    #ifdef __linux__
    data = stbi_load("../Source/Resources/Textures/diskscheduling.jpg", &width, &height, &nrChannels, 0);
    #elif _WIN32
    data = stbi_load("../../Source/Resources/Textures/diskscheduling.jpg", &width, &height, &nrChannels, 0);
    #endif
    if(data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        printf("Failed to load Texture\n");
    }
    stbi_image_free(data);

    float opt1[] = {
         0.5f,  0.15f, 0.0f,  1.0f,  1.0f,
         0.5f, -0.10f, 0.0f,  1.0f,  0.0f,
        -0.5f, -0.10f, 0.0f,  0.0f,  0.0f,
        -0.5f,  0.15f, 0.0f,  0.0f,  1.0f
    };

    glGenVertexArrays(1, &VAO[1]);
    glGenBuffers(1, &VBO[1]);
    //glGenBuffers(1, &EBO);

    glBindVertexArray(VAO[1]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(opt1), opt1, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //Texture 3
    glGenTextures(1, &texture[2]);

    glBindTexture(GL_TEXTURE_2D, texture[2]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(GL_TRUE);
    #ifdef __linux__
    data = stbi_load("../Source/Resources/Textures/pf.jpg", &width, &height, &nrChannels, 0);
    #elif _WIN32
    data = stbi_load("../../Source/Resources/Textures/pf.jpg", &width, &height, &nrChannels, 0);
    #endif
    if(data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        printf("Failed to load Texture\n");
    }
    stbi_image_free(data);

    float opt2[] = {
         0.5f, -0.25f, 0.0f,  1.0f,  1.0f,
         0.5f, -0.50f, 0.0f,  1.0f,  0.0f,
        -0.5f, -0.50f, 0.0f,  0.0f,  0.0f,
        -0.5f, -0.25f, 0.0f,  0.0f,  1.0f
    };

    glGenVertexArrays(1, &VAO[2]);
    glGenBuffers(1, &VBO[2]);
    //glGenBuffers(1, &EBO);

    glBindVertexArray(VAO[2]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(opt2), opt2, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    //Texture 4
    glGenTextures(1, &texture[3]);

    glBindTexture(GL_TEXTURE_2D, texture[3]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(GL_TRUE);
    #ifdef __linux__
    data = stbi_load("../Source/Resources/Textures/fragment.jpg", &width, &height, &nrChannels, 0);
    #elif _WIN32
    data = stbi_load("../../Source/Resources/Textures/fragment.jpg", &width, &height, &nrChannels, 0);
    #endif
    if(data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        printf("Failed to load Texture\n");
    }
    stbi_image_free(data);

    float opt3[] = {
         0.5f, -0.65f, 0.0f,  1.0f,  1.0f,
         0.5f, -0.90f, 0.0f,  1.0f,  0.0f,
        -0.5f, -0.90f, 0.0f,  0.0f,  0.0f,
        -0.5f, -0.65f, 0.0f,  0.0f,  1.0f
    };

    glGenVertexArrays(1, &VAO[3]);
    glGenBuffers(1, &VBO[3]);
    //glGenBuffers(1, &EBO);

    glBindVertexArray(VAO[3]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(opt3), opt3, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void back()
{
    GLint textureLocation = glGetUniformLocation(shaderProgram, "bgTex");
    glUseProgram(shaderProgram);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glBindVertexArray(VAO[0]);
    glUniform1i(textureLocation, 0);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glBindVertexArray(VAO[1]);
    glUniform1i(textureLocation, 0);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture[2]);
    glBindVertexArray(VAO[2]);
    glUniform1i(textureLocation, 0);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture[3]);
    glBindVertexArray(VAO[3]);
    glUniform1i(textureLocation, 0);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}


void initDisk()
{
    
    // link shaders
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        printf( "ERROR::SHADER::PROGRAM::LINKING_FAILED\n");
        printf ("%s\n", infoLog);
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    //Generating Textures
    
    int width, height, nrChannels;
    unsigned char *data;

    //Texture 1
    glGenTextures(1, &texture[4]);

    glBindTexture(GL_TEXTURE_2D, texture[4]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(GL_TRUE);
    #ifdef __linux__
    data = stbi_load("../Source/Resources/Textures/background.png", &width, &height, &nrChannels, 0);
    #elif _WIN32
    data = stbi_load("../../Source/Resources/Textures/background.png", &width, &height, &nrChannels, 0);
    #endif
    if(data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        printf("Failed to load Texture\n");
    }
    stbi_image_free(data);

    float bg[] = {
         1.0f,  1.0f, 0.0f,  1.0f,  1.0f,
         1.0f, -1.0f, 0.0f,  1.0f,  0.0f,
        -1.0f, -1.0f, 0.0f,  0.0f,  0.0f,
        -1.0f,  1.0f, 0.0f,  0.0f,  1.0f
    };

    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 2,  // first Triangle
        0, 2, 3   // second Triangle
    };

    glGenVertexArrays(1, &VAO[4]);
    glGenBuffers(1, &VBO[4]);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO[4]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[4]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(bg), bg, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //Texture 2
    glGenTextures(1, &texture[5]);

    glBindTexture(GL_TEXTURE_2D, texture[5]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(GL_TRUE);
    #ifdef __linux__
    data = stbi_load("../Source/Resources/Textures/fcfs.jpg", &width, &height, &nrChannels, 0);
    #elif _WIN32
    data = stbi_load("../../Source/Resources/Textures/fcfs.jpg", &width, &height, &nrChannels, 0);
    #endif
    if(data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        printf("Failed to load Texture\n");
    }
    stbi_image_free(data);

    float opt1[] = {
         0.75f,  0.90f, 0.0f,  1.0f,  1.0f,
         0.75f,  0.20f, 0.0f,  1.0f,  0.0f,
         0.10f,  0.20f, 0.0f,  0.0f,  0.0f,
         0.10f,  0.90f, 0.0f,  0.0f,  1.0f
    };

    glGenVertexArrays(1, &VAO[5]);
    glGenBuffers(1, &VBO[5]);
    //glGenBuffers(1, &EBO);

    glBindVertexArray(VAO[5]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[5]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(opt1), opt1, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //Texture 3
    glGenTextures(1, &texture[6]);

    glBindTexture(GL_TEXTURE_2D, texture[6]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(GL_TRUE);
    #ifdef __linux__
    data = stbi_load("../Source/Resources/Textures/sstf.jpg", &width, &height, &nrChannels, 0);
    #elif _WIN32
    data = stbi_load("../../Source/Resources/Textures/sstf.jpg", &width, &height, &nrChannels, 0);
    #endif
    if(data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        printf("Failed to load Texture\n");
    }
    stbi_image_free(data);

    float opt2[] = {
        -0.10f,  0.90f, 0.0f,  1.0f,  1.0f,
        -0.10f,  0.20f, 0.0f,  1.0f,  0.0f,
        -0.75f,  0.20f, 0.0f,  0.0f,  0.0f,
        -0.75f,  0.90f, 0.0f,  0.0f,  1.0f
    };

    glGenVertexArrays(1, &VAO[6]);
    glGenBuffers(1, &VBO[6]);
    //glGenBuffers(1, &EBO);

    glBindVertexArray(VAO[6]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[6]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(opt2), opt2, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //Texture 4
    glGenTextures(1, &texture[7]);

    glBindTexture(GL_TEXTURE_2D, texture[7]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(GL_TRUE);
    #ifdef __linux__
    data = stbi_load("../Source/Resources/Textures/look.jpg", &width, &height, &nrChannels, 0);
    #elif _WIN32
    data = stbi_load("../../Source/Resources/Textures/look.jpg", &width, &height, &nrChannels, 0);
    #endif
    if(data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        printf("Failed to load Texture\n");
    }
    stbi_image_free(data);

    float opt3[] = {
        -0.10f, -0.20f, 0.0f,  1.0f,  1.0f,
        -0.10f, -0.90f, 0.0f,  1.0f,  0.0f,
        -0.75f, -0.90f, 0.0f,  0.0f,  0.0f,
        -0.75f, -0.20f, 0.0f,  0.0f,  1.0f
    };

    glGenVertexArrays(1, &VAO[7]);
    glGenBuffers(1, &VBO[7]);
    //glGenBuffers(1, &EBO);

    glBindVertexArray(VAO[7]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[7]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(opt3), opt3, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //Texture 5
    glGenTextures(1, &texture[8]);

    glBindTexture(GL_TEXTURE_2D, texture[8]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(GL_TRUE);
    #ifdef __linux__
    data = stbi_load("../Source/Resources/Textures/scan.jpg", &width, &height, &nrChannels, 0);
    #elif _WIN32
    data = stbi_load("../../Source/Resources/Textures/scan.jpg", &width, &height, &nrChannels, 0);
    #endif
    if(data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        printf("Failed to load Texture\n");
    }
    stbi_image_free(data);

    float opt4[] = {
         0.75f, -0.20f, 0.0f,  1.0f,  1.0f,
         0.75f, -0.90f, 0.0f,  1.0f,  0.0f,
         0.10f, -0.90f, 0.0f,  0.0f,  0.0f,
         0.10f, -0.20f, 0.0f,  0.0f,  1.0f
    };

    glGenVertexArrays(1, &VAO[8]);
    glGenBuffers(1, &VBO[8]);
    //glGenBuffers(1, &EBO);

    glBindVertexArray(VAO[8]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[8]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(opt4), opt4, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void diskScreen()
{
    GLint textureLocation = glGetUniformLocation(shaderProgram, "bgTex");
    glUseProgram(shaderProgram);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture[4]);
    glBindVertexArray(VAO[4]);
    glUniform1i(textureLocation, 0);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture[5]);
    glBindVertexArray(VAO[5]);
    glUniform1i(textureLocation, 0);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture[6]);
    glBindVertexArray(VAO[6]);
    glUniform1i(textureLocation, 0);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture[7]);
    glBindVertexArray(VAO[7]);
    glUniform1i(textureLocation, 0);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture[8]);
    glBindVertexArray(VAO[8]);
    glUniform1i(textureLocation, 0);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void initFcfs()
{
    //compileShaderDiskAlgo();
    // link shaders
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        printf( "ERROR::SHADER::PROGRAM::LINKING_FAILED\n");
        printf ("%s\n", infoLog);
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    //Generating Textures
    
    int width, height, nrChannels;
    unsigned char *data;

    //Texture 1
    glGenTextures(1, &texture[9]);

    glBindTexture(GL_TEXTURE_2D, texture[9]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(GL_TRUE);
    #ifdef __linux__
    data = stbi_load("../Source/Resources/Textures/fcfs_alternate.jpg", &width, &height, &nrChannels, 0);
    #elif _WIN32
    data = stbi_load("../../Source/Resources/Textures/fcfs_alternate.jpg", &width, &height, &nrChannels, 0);
    #endif
    if(data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        printf("Failed to load Texture\n");
    }
    stbi_image_free(data);

    float bg[] = {
         1.0f,  1.0f, 0.0f,  1.0f,  1.0f,
         1.0f, -1.0f, 0.0f,  1.0f,  0.0f,
        -1.0f, -1.0f, 0.0f,  0.0f,  0.0f,
        -1.0f,  1.0f, 0.0f,  0.0f,  1.0f
    };

    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 2,  // first Triangle
        0, 2, 3   // second Triangle
    };

    glGenVertexArrays(1, &VAO[9]);
    glGenBuffers(1, &VBO[9]);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO[9]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[9]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(bg), bg, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

}

void initScan()
{
    
    // link shaders
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        printf( "ERROR::SHADER::PROGRAM::LINKING_FAILED\n");
        printf ("%s\n", infoLog);
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    //Generating Textures
    
    int width, height, nrChannels;
    unsigned char *data;

    //Texture 1
    glGenTextures(1, &texture[10]);

    glBindTexture(GL_TEXTURE_2D, texture[10]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(GL_TRUE);
    #ifdef __linux__
    data = stbi_load("../Source/Resources/Textures/scan_alternate.jpg", &width, &height, &nrChannels, 0);
    #elif _WIN32
    data = stbi_load("../../Source/Resources/Textures/scan_alternate.jpg", &width, &height, &nrChannels, 0);
    #endif
    if(data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        printf("Failed to load Texture\n");
    }
    stbi_image_free(data);

    float bg[] = {
         1.0f,  1.0f, 0.0f,  1.0f,  1.0f,
         1.0f, -1.0f, 0.0f,  1.0f,  0.0f,
        -1.0f, -1.0f, 0.0f,  0.0f,  0.0f,
        -1.0f,  1.0f, 0.0f,  0.0f,  1.0f
    };

    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 2,  // first Triangle
        0, 2, 3   // second Triangle
    };

    glGenVertexArrays(1, &VAO[10]);
    glGenBuffers(1, &VBO[10]);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO[10]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[10]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(bg), bg, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

}

void initSstf()
{
    
    // link shaders
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        printf( "ERROR::SHADER::PROGRAM::LINKING_FAILED\n");
        printf ("%s\n", infoLog);
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    //Generating Textures
    
    int width, height, nrChannels;
    unsigned char *data;

    //Texture 1
    glGenTextures(1, &texture[11]);

    glBindTexture(GL_TEXTURE_2D, texture[11]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(GL_TRUE);
    #ifdef __linux__
    data = stbi_load("../Source/Resources/Textures/sstf_alternate.jpg", &width, &height, &nrChannels, 0);
    #elif _WIN32
    data = stbi_load("../../Source/Resources/Textures/sstf_alternate.jpg", &width, &height, &nrChannels, 0);
    #endif
    if(data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        printf("Failed to load Texture\n");
    }
    stbi_image_free(data);

    float bg[] = {
         1.0f,  1.0f, 0.0f,  1.0f,  1.0f,
         1.0f, -1.0f, 0.0f,  1.0f,  0.0f,
        -1.0f, -1.0f, 0.0f,  0.0f,  0.0f,
        -1.0f,  1.0f, 0.0f,  0.0f,  1.0f
    };

    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 2,  // first Triangle
        0, 2, 3   // second Triangle
    };

    glGenVertexArrays(1, &VAO[11]);
    glGenBuffers(1, &VBO[11]);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO[11]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[11]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(bg), bg, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

}

void initLook()
{
    
    // link shaders
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        printf( "ERROR::SHADER::PROGRAM::LINKING_FAILED\n");
        printf ("%s\n", infoLog);
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    //Generating Textures
    
    int width, height, nrChannels;
    unsigned char *data;

    //Texture 1
    glGenTextures(1, &texture[12]);

    glBindTexture(GL_TEXTURE_2D, texture[12]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(GL_TRUE);
    #ifdef __linux__
    data = stbi_load("../Source/Resources/Textures/look_alternate.jpg", &width, &height, &nrChannels, 0);
    #elif _WIN32
    data = stbi_load("../../Source/Resources/Textures/look_alternate.jpg", &width, &height, &nrChannels, 0);
    #endif
    if(data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        printf("Failed to load Texture\n");
    }
    stbi_image_free(data);

    float bg[] = {
         1.0f,  1.0f, 0.0f,  1.0f,  1.0f,
         1.0f, -1.0f, 0.0f,  1.0f,  0.0f,
        -1.0f, -1.0f, 0.0f,  0.0f,  0.0f,
        -1.0f,  1.0f, 0.0f,  0.0f,  1.0f
    };

    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 2,  // first Triangle
        0, 2, 3   // second Triangle
    };

    glGenVertexArrays(1, &VAO[12]);
    glGenBuffers(1, &VBO[12]);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO[12]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[12]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(bg), bg, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

}

void dispFcfs()
{
    GLint textureLocation = glGetUniformLocation(shaderProgram, "bgTex");
    glUseProgram(shaderProgram);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture[9]);
    glBindVertexArray(VAO[9]);
    glUniform1i(textureLocation, 0);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    diskFcfs();
}

void dispScan()
{
    GLint textureLocation = glGetUniformLocation(shaderProgram, "bgTex");
    glUseProgram(shaderProgram);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture[10]);
    glBindVertexArray(VAO[10]);
    glUniform1i(textureLocation, 0);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    diskScan();
}

void dispSstf()
{
    GLint textureLocation = glGetUniformLocation(shaderProgram, "bgTex");
    glUseProgram(shaderProgram);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture[11]);
    glBindVertexArray(VAO[11]);
    glUniform1i(textureLocation, 0);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    diskSstf();
}

void dispLook()
{
    GLint textureLocation = glGetUniformLocation(shaderProgram, "bgTex");
    glUseProgram(shaderProgram);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture[12]);
    glBindVertexArray(VAO[12]);
    glUniform1i(textureLocation, 0);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    diskLook();
}

void initPageRep()
{
    //compileShaderDiskAlgo();
    // link shaders
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        printf( "ERROR::SHADER::PROGRAM::LINKING_FAILED\n");
        printf ("%s\n", infoLog);
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    //Generating Textures
    
    int width, height, nrChannels;
    unsigned char *data;

    //Texture 1
    glGenTextures(1, &texture[14]);

    glBindTexture(GL_TEXTURE_2D, texture[14]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(GL_TRUE);
    #ifdef __linux__
    data = stbi_load("../Source/Resources/Textures/PageReplace.jpg", &width, &height, &nrChannels, 0);
    #elif _WIN32
    data = stbi_load("../../Source/Resources/Textures/PageReplace.jpg", &width, &height, &nrChannels, 0);
    #endif
    if(data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        printf("Failed to load Texture\n");
    }
    stbi_image_free(data);

    float bg[] = {
         1.0f,  1.0f, 0.0f,  1.0f,  1.0f,
         1.0f, -1.0f, 0.0f,  1.0f,  0.0f,
        -1.0f, -1.0f, 0.0f,  0.0f,  0.0f,
        -1.0f,  1.0f, 0.0f,  0.0f,  1.0f
    };

    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 2,  // first Triangle
        0, 2, 3   // second Triangle
    };

    glGenVertexArrays(1, &VAO[14]);
    glGenBuffers(1, &VBO[14]);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO[14]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[14]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(bg), bg, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

}

void dispPageRep()
{
    GLint textureLocation = glGetUniformLocation(shaderProgram, "bgTex");
    glUseProgram(shaderProgram);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture[14]);
    glBindVertexArray(VAO[14]);
    glUniform1i(textureLocation, 0);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}