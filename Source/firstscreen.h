    #include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#include "Resources/Headers/stb_image.h"

const char *vertexShaderSource ="#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aColor;\n"
    "out vec3 ourColor;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos, 1.0);\n"
    "   ourColor = aColor;\n"
    "}\0";

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

//Fragment Shader1
const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec3 ourColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(ourColor, 1.0f);\n"
    "}\n\0";

//Fragment Shader2(background)
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

int shaderProgram, optShader1, optShader2, optShader3;
unsigned int VBO[4], VAO[4], EBO;
unsigned int texture[4];

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
        printf("Failed to load Texture");
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
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
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
        printf("Failed to load Texture");
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
    glGenBuffers(1, &EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
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
        printf("Failed to load Texture");
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
    glGenBuffers(1, &EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
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
        printf("Failed to load Texture");
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
    glGenBuffers(1, &EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
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

void initOpt3()
{
    // link shaders
    optShader3 = glCreateProgram();
    glAttachShader(optShader3, vertexShader);
    glAttachShader(optShader3, fragmentShader);
    glLinkProgram(optShader3);
    // check for linking errors
    glGetProgramiv(optShader3, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(optShader3, 512, NULL, infoLog);
        printf( "ERROR::SHADER::PROGRAM::LINKING_FAILED\n");
        printf ("%s\n", infoLog);
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    //Generating Textures
    
    int width, height, nrChannels;
    unsigned char *data;

    //Texture 4
    glGenTextures(1, &texture[3]);

    glBindTexture(GL_TEXTURE_2D, texture[3]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(GL_TRUE);
    #ifdef __linux__
    data = stbi_load("../Source/Resources/Textures/3.png", &width, &height, &nrChannels, 0);
    #elif _WIN32
    data = stbi_load("../../Source/Resources/Textures/3.png", &width, &height, &nrChannels, 0);
    #endif
    if(data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        printf("Failed to load Texture");
    }
    stbi_image_free(data);

    float opt3[] = {
         0.5f, -0.65f, 0.0f,  1.0f,  1.0f,
         0.5f, -0.90f, 0.0f,  1.0f,  0.0f,
        -0.5f, -0.90f, 0.0f,  0.0f,  0.0f,
        -0.5f, -0.65f, 0.0f,  0.0f,  1.0f
    };

    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 2,  // first Triangle
        0, 2, 3   // second Triangle
    };

    glGenVertexArrays(1, &VAO[3]);
    glGenBuffers(1, &VBO[3]);
    glGenBuffers(1, &EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
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

void opt3a()
{
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, texture[3]);
    glBindVertexArray(VAO[3]);
    glUseProgram(optShader3);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}