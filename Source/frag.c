#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>

#include <kazmath/kazmath.h>
#include "rendertext.h"

const char *vertexFragShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aColor;\n"
    "out vec3 ourColor;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "ourColor = aColor;\n"
    "}\0";
const char *fragmentFragShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec3 ourColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(ourColor, 1.0f);\n"
    "}\n\0";

void frag()
{
    // vertex shader
    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexFragShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n");
    }
    // fragment shader
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentFragShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n");
    }
    // link shaders
    int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        printf("ERROR::SHADER::PROGRAM::LINKING_FAILED\n");
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
         0.28f, 0.9f, 0.0f,     0.6f, 0.0f, 0.0f, //0
         0.28f, 0.7f, 0.0f,     0.6f, 0.0f, 0.0f, //1
         0.2f,  0.7f, 0.0f,     0.6f, 0.0f, 0.0f, //2
         0.2f,  0.9f, 0.0f,     0.6f, 0.0f, 0.0f, //3
         0.38f, 0.9f, 0.0f,     0.6f, 0.0f, 0.0f, //4
         0.38f, 0.7f, 0.0f,     0.6f, 0.0f, 0.0f, //5
         0.30f, 0.7f, 0.0f,     0.6f, 0.0f, 0.0f, //6
         0.30f, 0.9f, 0.0f,     0.6f, 0.0f, 0.0f, //7
         0.48f, 0.9f, 0.0f,     0.0f, 0.6f, 0.0f, //8
         0.48f, 0.7f, 0.0f,     0.0f, 0.6f, 0.0f, //9
         0.40f, 0.7f, 0.0f,     0.0f, 0.6f, 0.0f, //10
         0.40f, 0.9f, 0.0f,     0.0f, 0.6f, 0.0f, //11
         0.58f, 0.9f, 0.0f,     0.0f, 0.0f, 0.6f, //12
         0.58f, 0.7f, 0.0f,     0.0f, 0.0f, 0.6f, //13
         0.50f, 0.7f, 0.0f,     0.0f, 0.0f, 0.6f, //14
         0.50f, 0.9f, 0.0f,     0.0f, 0.0f, 0.6f, //15
         0.68f, 0.9f, 0.0f,     0.6f, 0.0f, 0.0f, //16
         0.68f, 0.7f, 0.0f,     0.6f, 0.0f, 0.0f, //17
         0.60f, 0.7f, 0.0f,     0.6f, 0.0f, 0.0f, //18
         0.60f, 0.9f, 0.0f,     0.6f, 0.0f, 0.0f, //19
         0.28f, 0.65f, 0.0f,    0.0f, 0.0f, 0.6f, //20
         0.28f, 0.45f, 0.0f,     0.0f, 0.0f, 0.6f, //21
         0.2f,  0.45f, 0.0f,     0.0f, 0.0f, 0.6f, //22
         0.2f,  0.65f, 0.0f,    0.0f, 0.0f, 0.6f, //23
         0.38f, 0.65f, 0.0f,    0.0f, 0.6f, 0.0f, //24
         0.38f, 0.45f, 0.0f,     0.0f, 0.6f, 0.0f, //25
         0.30f, 0.45f, 0.0f,     0.0f, 0.6f, 0.0f, //26
         0.30f, 0.65f, 0.0f,    0.0f, 0.6f, 0.0f, //27
         0.48f, 0.65f, 0.0f,    0.0f, 0.0f, 0.6f, //28
         0.48f, 0.45f, 0.0f,     0.0f, 0.0f, 0.6f, //29
         0.40f, 0.45f, 0.0f,     0.0f, 0.0f, 0.6f, //30
         0.40f, 0.65f, 0.0f,    0.0f, 0.0f, 0.6f, //31
         0.58f, 0.65f, 0.0f,    0.0f, 0.6f, 0.0f, //32
         0.58f, 0.45f, 0.0f,     0.0f, 0.6f, 0.0f, //33
         0.50f, 0.45f, 0.0f,     0.0f, 0.6f, 0.0f, //34
         0.50f, 0.65f, 0.0f,    0.0f, 0.6f, 0.0f, //35
         0.68f, 0.65f, 0.0f,    0.6f, 0.0f, 0.0f, //36
         0.68f, 0.45f, 0.0f,     0.6f, 0.0f, 0.0f, //37
         0.60f, 0.45f, 0.0f,     0.6f, 0.0f, 0.0f, //38
         0.60f, 0.65f, 0.0f,    0.6f, 0.0f, 0.0f, //39
         0.28f, 0.2f, 0.0f,     0.6f, 0.0f, 0.0f, //40
         0.28f, 0.0f, 0.0f,     0.6f, 0.0f, 0.0f, //41
         0.2f,  0.0f, 0.0f,     0.6f, 0.0f, 0.0f, //42
         0.2f,  0.2f, 0.0f,     0.6f, 0.0f, 0.0f, //43
         0.38f, 0.2f, 0.0f,     0.6f, 0.0f, 0.0f, //44
         0.38f, 0.0f, 0.0f,     0.6f, 0.0f, 0.0f, //45
         0.30f, 0.0f, 0.0f,     0.6f, 0.0f, 0.0f, //46
         0.30f, 0.2f, 0.0f,     0.6f, 0.0f, 0.0f, //47
         0.48f, 0.2f, 0.0f,     0.6f, 0.0f, 0.0f, //48
         0.48f, 0.0f, 0.0f,     0.6f, 0.0f, 0.0f, //49
         0.40f, 0.0f, 0.0f,     0.6f, 0.0f, 0.0f, //50
         0.40f, 0.2f, 0.0f,     0.6f, 0.0f, 0.0f, //51
         0.58f, 0.2f, 0.0f,     0.6f, 0.0f, 0.0f, //52
         0.58f, 0.0f, 0.0f,     0.6f, 0.0f, 0.0f, //53
         0.50f, 0.0f, 0.0f,     0.6f, 0.0f, 0.0f, //54
         0.50f, 0.2f, 0.0f,     0.6f, 0.0f, 0.0f, //55
         0.68f, 0.2f, 0.0f,     0.0f, 0.6f, 0.0f, //56
         0.68f, 0.0f, 0.0f,     0.0f, 0.6f, 0.0f, //57
         0.60f, 0.0f, 0.0f,     0.0f, 0.6f, 0.0f, //58
         0.60f, 0.2f, 0.0f,     0.0f, 0.6f, 0.0f, //59
         0.28f, -0.05f, 0.0f,    0.0f, 0.6f, 0.0f, //60
         0.28f, -0.25f, 0.0f,     0.0f, 0.6f, 0.0f, //61
         0.2f,  -0.25f, 0.0f,     0.0f, 0.6f, 0.0f, //62
         0.2f,  -0.05f, 0.0f,    0.0f, 0.6f, 0.0f, //63
         0.38f, -0.05f, 0.0f,    0.0f, 0.6f, 0.0f, //64
         0.38f, -0.25f, 0.0f,     0.0f, 0.6f, 0.0f, //65
         0.30f, -0.25f, 0.0f,     0.0f, 0.6f, 0.0f, //66
         0.30f, -0.05f, 0.0f,    0.0f, 0.6f, 0.0f, //67
         0.48f, -0.05f, 0.0f,    0.0f, 0.0f, 0.6f, //68
         0.48f, -0.25f, 0.0f,     0.0f, 0.0f, 0.6f, //69
         0.40f, -0.25f, 0.0f,     0.0f, 0.0f, 0.6f, //70
         0.40f, -0.05f, 0.0f,    0.0f, 0.0f, 0.6f, //71
         0.58f, -0.05f, 0.0f,    0.0f, 0.0f, 0.6f, //72
         0.58f, -0.25f, 0.0f,     0.0f, 0.0f, 0.6f, //73
         0.50f, -0.25f, 0.0f,     0.0f, 0.0f, 0.6f, //74
         0.50f, -0.05f, 0.0f,    0.0f, 0.0f, 0.6f, //75
         0.68f, -0.05f, 0.0f,    0.0f, 0.0f, 0.6f, //76
         0.68f, -0.25f, 0.0f,     0.0f, 0.0f, 0.6f, //77
         0.60f, -0.25f, 0.0f,     0.0f, 0.0f, 0.6f, //78
         0.60f, -0.05f, 0.0f,    0.0f, 0.0f, 0.6f, //79
         0.28f, -0.4f,  0.0f,     0.6f, 0.0f, 0.0f, //80 //cluster names//
         0.28f, -0.55f, 0.0f,     0.6f, 0.0f, 0.0f, //81
         0.2f,  -0.55f, 0.0f,     0.6f, 0.0f, 0.0f, //82
         0.2f,  -0.4f,  0.0f,     0.6f, 0.0f, 0.0f, //83
         0.28f, -0.6f,  0.0f,     0.0f, 0.6f, 0.0f, //84
         0.28f, -0.75f, 0.0f,     0.0f, 0.6f, 0.0f, //85
         0.2f,  -0.75f, 0.0f,     0.0f, 0.6f, 0.0f, //86
         0.2f,  -0.6f,  0.0f,     0.0f, 0.6f, 0.0f, //87
         0.28f, -0.8f, 0.0f,      0.0f, 0.0f, 0.6f, //88
         0.28f, -0.95f, 0.0f,     0.0f, 0.0f, 0.6f, //89
         0.20f, -0.95f, 0.0f,     0.0f, 0.0f, 0.6f, //90
         0.20f, -0.8f, 0.0f,      0.0f, 0.0f, 0.6f, //91
         
         
    };
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,
        1, 2, 3,
        4, 5, 7,
        5, 6, 7,
        8, 9, 11,
        9, 10, 11,
        12, 13, 15,
        13, 14, 15,
        16, 17, 19,
        17, 18, 19,
        20, 21, 23,
        21, 22, 23,
        24, 25, 27,
        25, 26, 27,
        28, 29, 31,
        29, 30, 31,
        32, 33, 35,
        33, 34, 35,
        36, 37, 39,
        37, 38, 39,
        40, 41, 43,
        41, 42, 43,
        44, 45, 47,
        45, 46, 47,
        48, 49, 51,
        49, 50, 51,
        52, 53, 55,
        53, 54, 55,
        56, 57, 59,
        57, 58, 59,
        60, 61, 63,
        61, 62, 63,
        64, 65, 67,
        65, 66, 67,
        68, 69, 71,
        69, 70, 71,
        72, 73, 75,
        73, 74, 75,
        76, 77, 79,
        77, 78, 79,
        80, 81, 83,
        81, 82, 83,
        84, 85, 87,
        85, 86, 87,
        88, 89, 91,
        89, 90, 91
    };
    float vertices1 [] = {
        //box1//
        0.18f, 0.92f, 0.0f,     1.0f, 1.0f, 1.0f, //0
        0.70f, 0.92f, 0.0f,     1.0f, 1.0f, 1.0f, //1
        0.70f, 0.43f, 0.0f,     1.0f, 1.0f, 1.0f, //2
        0.18f, 0.43f, 0.0f,     1.0f, 1.0f, 1.0f, //3
        
        //box2//
        0.18f, 0.22f, 0.0f,     1.0f, 1.0f, 1.0f, //4
        0.70f, 0.22f, 0.0f,     1.0f, 1.0f, 1.0f, //5
        0.70f, -0.27f, 0.0f,    1.0f, 1.0f, 1.0f, //6
        0.18f, -0.27f, 0.0f,    1.0f, 1.0f, 1.0f //7
    };
    unsigned int VBObdry, VAObdry, EBObdry;
    unsigned int VBO1, VAO1, EBO1;
    glGenVertexArrays(1, &VAObdry);
    glGenBuffers(1, &VBObdry);
    glGenBuffers(1, &EBObdry);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAObdry);

    glBindBuffer(GL_ARRAY_BUFFER, VBObdry);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBObdry);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glGenVertexArrays(1, &VAO1);
    glGenBuffers(1, &VBO1);
    //glGenBuffers(1, &EBO1);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO1);

    glBindBuffer(GL_ARRAY_BUFFER, VBO1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0); 

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw our first triangle
        glUseProgram(shaderProgram);
        glBindVertexArray(VAObdry); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        //glDrawArrays(GL_TRIANGLES, 0, 6);
        glDrawElements(GL_TRIANGLES, 138, GL_UNSIGNED_INT, 0);

        glBindVertexArray(VAO1);
        glDrawArrays(GL_LINE_LOOP,0,4);
        glDrawArrays(GL_LINE_LOOP,4,4);
        // glBindVertexArray(0); // no need to unbind it every time 

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAObdry);
    glDeleteBuffers(1, &VBObdry);
    glDeleteBuffers(1, &EBObdry);
}