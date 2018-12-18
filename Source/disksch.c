#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include <kazmath/kazmath.h>

#include "Resources/Headers/stb_image.h"
#include "rendertext.h"
#include "disksch.h"

//Vertex BG Shader
const char *vertexShaderSource ="#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aColor;\n"
    "out vec3 ourColor;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos, 1.0);\n"
        "ourColor = aColor;\n"
    "}\0";

//Fragment Shader
const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec3 ourColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(ourColor, 1.0f);\n"
    "}\n\0";

int shaderProgramTex, queueList[5],queue1[10], queue2[10], queueScan[10], queueMax[10], queueMin[10],queueLook[10], head, temp, temp1 = 0, temp2 = 0;
unsigned int VBOTEX, VAOTEX, EBOTEX, VBOVERT, VAOVERT;
unsigned int textureTexBlue, textureTexYellow;

float xPos1, xPos2, yPos1, yPos2;

int vertexShaderTex, fragmentShaderTex;
int success;
char infoLog[512];

void compileShaderDiskAlgo()
{
    // vertex shader
    vertexShaderTex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShaderTex, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShaderTex);
    // check for shader compile errors
    success;
    infoLog[512];
    glGetShaderiv(vertexShaderTex, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShaderTex, 512, NULL, infoLog);
        printf ("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n");
        printf ("%s\n", infoLog);

    }
    // fragment shader
    fragmentShaderTex = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderTex, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShaderTex);
    // check for shader compile errors
    glGetShaderiv(fragmentShaderTex, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShaderTex, 512, NULL, infoLog);
        printf ("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n");
        printf ("%s\n", infoLog);
    }

    shaderProgramTex = glCreateProgram();
    glAttachShader(shaderProgramTex, vertexShaderTex);
    glAttachShader(shaderProgramTex, fragmentShaderTex);
    glLinkProgram(shaderProgramTex);
    // check for linking errors
    glGetProgramiv(shaderProgramTex, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgramTex, 512, NULL, infoLog);
        printf( "ERROR::SHADER::PROGRAM::LINKING_FAILED\n");
        printf ("%s\n", infoLog);
    }
    glDeleteShader(vertexShaderTex);
    glDeleteShader(fragmentShaderTex);

    srand(time(NULL));
    for(int i=0; i<5; i++)
    {
        queueList[i] = rand() % 100;
        queue1[i] = queueList[i];
        queue2[i] = queueList[i];
    }
    head = rand() % 100;

    //Scan Queue
    queue1[5] = head;
    for(int i=0; i<6;i++)
    {
        for(int j=i; j<6; j++)
        {
            if(queue1[i]>queue1[j])
            {
                temp = queue1[i];
                queue1[i] = queue1[j];
                queue1[j] = temp;
            }
        }
    }

    int headLoc;

    for(int i=0; i<6; i++)
    {
        if(head == queue1[i]){
            headLoc = i;
            break;
        }
    }

    if(abs(head) <= abs(head-99))
    {  
        int i=0;
        /*Dloc tracks head position and is updated accordingly*/
        for(int j=headLoc; j>=0; j--)
        {
            queueScan[i] = queue1[j];
            i++;
        }
        queueScan[i] = 0;
        i++;
        for(int j=headLoc+1; j<6; j++)
        {
            queueScan[i] = queue1[j];
            i++;
        }
    } 
    else
    {
        int i = 0;
        for(int j=headLoc+1; j<6; j++)
        {
            queueScan[i] = queue1[j];
            i++;
        }
        queueScan[i] = 99;
        i++;
        for(int j=headLoc; j>=0; j--)
        {
            queueScan[i] = queue1[j];
            i++;
        }
    }

    //Look Queue
    int i, j;
    for(i=0; i<5; i++)
    {
        //queueMax - elems greater than head
        if(queue2[i] >= head)
        {
            queueMax[temp1] = temp;
            temp1++;
        } 
        else 
        {
            queueMin[temp2] = temp;
            temp2++;
        }
    }

    //sort queueMax - increasing order
    for(i=0; i<temp1-1; i++)
    {
        for(j=i+1; j<temp1; j++)
        {
            if(queueMax[i] > queueMax[j])
            {
                temp = queue1[i];
                queue1[i] = queue1[j];
                queue1[j] = temp;
            }
        }
    }
    //sort queueMin - decreasing order
    for(i=0; i<temp2-1; i++)
    {
        for(j=i+1; j<temp2; j++)
        {
            if(queueMin[i] < queueMin[j])
            {
                temp = queueMin[i];
                queueMin[i] = queueMin[j];
                queueMin[j] = temp;
            }
        }
    }

    if(abs(head) >= abs(head-99))
    {
        for(i=1,j=0; j<temp1; i++,j++)
        {
          queueLook[i] = queueMax[j];
        }
        for(i=temp1+1, j=0; j<temp2; i++, j++)
        {
          queueLook[i] = queueMin[j];
        }
    } 
    else 
    {
        for(i=1,j=0; j<temp2; i++,j++)
        {
            queueLook[i] = queueMin[j];
        } 
        for(i=temp2+1, j=0; j<temp1; i++, j++)
        {
            queueLook[i] = queueMax[j];
        }
    }
    queueLook[0] = head;
}

void diskFcfs()
{
    glUseProgram(shaderProgramTex);

    //kmMat4 projection;
    //kmMat4OrthographicProjection(&projection, 0.0f, 800.0f, 0.0f, 450.0f, 0.0f, 100.0f);
    //float *projPointer = valPtr(&projection);
    //glUniformMatrix4fv(glGetUniformLocation(shaderProgramText, "projection"), 1, GL_FALSE, projPointer);

    float rColor[5], gColor[5], bColor[5];

    for (int i=0; i<5; i++)
    {
        rColor[i] = 0.0f;
        gColor[i] = 0.0f;
        bColor[i] = 1.0f;
    }

    float vertices [] = {
        //first
        0.14f, -0.75f, 0.00f,   1.0f, 1.0f, 1.0f,
        0.14f,  0.25f, 0.00f,   1.0f, 1.0f, 1.0f,
        0.15f,  0.25f, 0.00f,   1.0f, 1.0f, 1.0f,
        0.15f, -0.75f, 0.00f,   1.0f, 1.0f, 1.0f,
        //second
        0.14f,  0.26f, 0.00f,   1.0f, 1.0f, 1.0f,
        0.82f,  0.26f, 0.00f,   1.0f, 1.0f, 1.0f,
        0.82f,  0.25f, 0.00f,   1.0f, 1.0f, 1.0f,
        0.14f,  0.25f, 0.00f,   1.0f, 1.0f, 1.0f,
        //third
        0.82f,  0.26f, 0.00f,   1.0f, 1.0f, 1.0f,
        0.83f,  0.26f, 0.00f,   1.0f, 1.0f, 1.0f,
        0.83f, -0.75f, 0.00f,   1.0f, 1.0f, 1.0f,
        0.82f, -0.75f, 0.00f,   1.0f, 1.0f, 1.0f,
        //I/O-1
        0.19f,  0.55f, 0.00f,   rColor[0], gColor[0], bColor[0],
        0.28f,  0.55f, 0.00f,   rColor[0], gColor[0], bColor[0],
        0.28f,  0.45f, 0.00f,   rColor[0], gColor[0], bColor[0],
        0.19f,  0.45f, 0.00f,   rColor[0], gColor[0], bColor[0],
        //I/O-2
        0.31f,  0.55f, 0.00f,   rColor[1], gColor[1], bColor[1],
        0.40f,  0.55f, 0.00f,   rColor[1], gColor[1], bColor[1],
        0.40f,  0.45f, 0.00f,   rColor[1], gColor[1], bColor[1],
        0.31f,  0.45f, 0.00f,   rColor[1], gColor[1], bColor[1],
        //I/O-3
        0.43f,  0.55f, 0.00f,   rColor[2], gColor[2], bColor[2],
        0.52f,  0.55f, 0.00f,   rColor[2], gColor[2], bColor[2],
        0.52f,  0.45f, 0.00f,   rColor[2], gColor[2], bColor[2],
        0.43f,  0.45f, 0.00f,   rColor[2], gColor[2], bColor[2],
        //I/O-4
        0.55f,  0.55f, 0.00f,   rColor[3], gColor[3], bColor[3],
        0.65f,  0.55f, 0.00f,   rColor[3], gColor[3], bColor[3],
        0.65f,  0.45f, 0.00f,   rColor[3], gColor[3], bColor[3],
        0.55f,  0.45f, 0.00f,   rColor[3], gColor[3], bColor[3],
        //I/O-5
        0.68f,  0.55f, 0.00f,   rColor[4], gColor[4], bColor[4],
        0.77f,  0.55f, 0.00f,   rColor[4], gColor[4], bColor[4],
        0.77f,  0.45f, 0.00f,   rColor[4], gColor[4], bColor[4],
        0.68f,  0.45f, 0.00f,   rColor[4], gColor[4], bColor[4]
    };

    unsigned int indices [] = {
        //first
        0, 1, 3,
        1, 2, 3,
        //second
        4, 5, 6,
        4, 6, 7,
        //third
        8, 9, 10,
        8, 10, 11,
        //I/O-1
        12, 13, 14,
        12, 14, 15,
        //I/O-2
        16, 17, 18,
        16, 18, 19,
        //I/O-3
        20, 21, 22,
        20, 22, 23,
        //I/O-4
        24, 25, 26,
        24, 26, 27,
        //I/O-5
        28, 29, 30,
        28, 30, 31
    };

    // Configure VAO/VBO for texture quads
    glGenVertexArrays(1, &VAOTEX);
    glGenBuffers(1, &VBOTEX);
    glGenBuffers(1, &EBOTEX);

    glBindVertexArray(VAOTEX);
    glBindBuffer(GL_ARRAY_BUFFER, VBOTEX);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 2 * 3, NULL, GL_DYNAMIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
    //glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOTEX);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glUseProgram(shaderProgramTex);
    glBindBuffer(GL_ARRAY_BUFFER, VBOTEX);
    glDrawElements(GL_TRIANGLES, 48, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    //glDrawArrays(GL_TRIANGLES, 0, 2);
    
    kmVec3 color;
    color.x = 0.0f;
    color.y = 0.0f;
    color.z = 0.0f;
    renderText("0", 453.0f, 287.0f, 0.35f, color);
    renderText("50", 578.0f, 287.0f, 0.35f, color);
    renderText("99", 710.0f, 287.0f, 0.35f, color);
    renderText("Head :", 465.0f, 50.0f, 0.7f, color);
    char headText[10];
    itoa(head, headText);
    renderText(headText, 580.0f, 50.0f, 0.7f, color);
    
    float texPos = 480.0f;
    for (int i = 0; i < 5; i++)
    {
        char text[10];
        itoa(queueList[i], text);
        renderText(text, texPos, 330.0f, 0.45f, color);
        texPos += 50.0f;
    }

    // Configure VAO/VBO for texture quads
    
    float xOffset = 0.14f;
    float mult = 0.0068f;
    xPos1 = xOffset + head * mult;
    yPos1 = 0.15f;
    float yOffset = 0.10f;

    float vert [36] = {
            xPos1, yPos1, 0.0f, 1.0f, 1.0f, 1.0f,
        };

    for (int i=0; i<5; i++)
    {
        xPos1 = xOffset + queueList[i] * mult;
        yPos1 = yPos1 - 0.10f;

        vert[i+6+5*i] = xPos1;
        vert[i+7+5*i] = yPos1;
        vert[i+8+5*i] = 0.0f;
        vert[i+9+5*i] = 1.0f;
        vert[i+10+5*i] = 1.0f;
        vert[i+11+5*i] = 1.0f;
    }

    glGenVertexArrays(1, &VAOVERT);
    glGenBuffers(1, &VBOVERT);
    glBindVertexArray(VAOVERT);
    glBindBuffer(GL_ARRAY_BUFFER, VBOVERT);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vert), vert, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    glUseProgram(shaderProgramTex);

    glDrawArrays(GL_LINE_STRIP, 0, 6);
    
}

void diskSstf()
{
    glUseProgram(shaderProgramTex);

    //kmMat4 projection;
    //kmMat4OrthographicProjection(&projection, 0.0f, 800.0f, 0.0f, 450.0f, 0.0f, 100.0f);
    //float *projPointer = valPtr(&projection);
    //glUniformMatrix4fv(glGetUniformLocation(shaderProgramText, "projection"), 1, GL_FALSE, projPointer);

    float rColor[5], gColor[5], bColor[5];

    for (int i=0; i<5; i++)
    {
        rColor[i] = 0.0f;
        gColor[i] = 0.0f;
        bColor[i] = 1.0f;
    }

    float vertices [] = {
        //first
        0.14f, -0.75f, 0.00f,   1.0f, 1.0f, 1.0f,
        0.14f,  0.25f, 0.00f,   1.0f, 1.0f, 1.0f,
        0.15f,  0.25f, 0.00f,   1.0f, 1.0f, 1.0f,
        0.15f, -0.75f, 0.00f,   1.0f, 1.0f, 1.0f,
        //second
        0.14f,  0.26f, 0.00f,   1.0f, 1.0f, 1.0f,
        0.82f,  0.26f, 0.00f,   1.0f, 1.0f, 1.0f,
        0.82f,  0.25f, 0.00f,   1.0f, 1.0f, 1.0f,
        0.14f,  0.25f, 0.00f,   1.0f, 1.0f, 1.0f,
        //third
        0.82f,  0.26f, 0.00f,   1.0f, 1.0f, 1.0f,
        0.83f,  0.26f, 0.00f,   1.0f, 1.0f, 1.0f,
        0.83f, -0.75f, 0.00f,   1.0f, 1.0f, 1.0f,
        0.82f, -0.75f, 0.00f,   1.0f, 1.0f, 1.0f,
        //I/O-1
        0.19f,  0.55f, 0.00f,   rColor[0], gColor[0], bColor[0],
        0.28f,  0.55f, 0.00f,   rColor[0], gColor[0], bColor[0],
        0.28f,  0.45f, 0.00f,   rColor[0], gColor[0], bColor[0],
        0.19f,  0.45f, 0.00f,   rColor[0], gColor[0], bColor[0],
        //I/O-2
        0.31f,  0.55f, 0.00f,   rColor[1], gColor[1], bColor[1],
        0.40f,  0.55f, 0.00f,   rColor[1], gColor[1], bColor[1],
        0.40f,  0.45f, 0.00f,   rColor[1], gColor[1], bColor[1],
        0.31f,  0.45f, 0.00f,   rColor[1], gColor[1], bColor[1],
        //I/O-3
        0.43f,  0.55f, 0.00f,   rColor[2], gColor[2], bColor[2],
        0.52f,  0.55f, 0.00f,   rColor[2], gColor[2], bColor[2],
        0.52f,  0.45f, 0.00f,   rColor[2], gColor[2], bColor[2],
        0.43f,  0.45f, 0.00f,   rColor[2], gColor[2], bColor[2],
        //I/O-4
        0.55f,  0.55f, 0.00f,   rColor[3], gColor[3], bColor[3],
        0.65f,  0.55f, 0.00f,   rColor[3], gColor[3], bColor[3],
        0.65f,  0.45f, 0.00f,   rColor[3], gColor[3], bColor[3],
        0.55f,  0.45f, 0.00f,   rColor[3], gColor[3], bColor[3],
        //I/O-5
        0.68f,  0.55f, 0.00f,   rColor[4], gColor[4], bColor[4],
        0.77f,  0.55f, 0.00f,   rColor[4], gColor[4], bColor[4],
        0.77f,  0.45f, 0.00f,   rColor[4], gColor[4], bColor[4],
        0.68f,  0.45f, 0.00f,   rColor[4], gColor[4], bColor[4]
    };

    unsigned int indices [] = {
        //first
        0, 1, 3,
        1, 2, 3,
        //second
        4, 5, 6,
        4, 6, 7,
        //third
        8, 9, 10,
        8, 10, 11,
        //I/O-1
        12, 13, 14,
        12, 14, 15,
        //I/O-2
        16, 17, 18,
        16, 18, 19,
        //I/O-3
        20, 21, 22,
        20, 22, 23,
        //I/O-4
        24, 25, 26,
        24, 26, 27,
        //I/O-5
        28, 29, 30,
        28, 30, 31
    };

    // Configure VAO/VBO for texture quads
    glGenVertexArrays(1, &VAOTEX);
    glGenBuffers(1, &VBOTEX);
    glGenBuffers(1, &EBOTEX);

    glBindVertexArray(VAOTEX);
    glBindBuffer(GL_ARRAY_BUFFER, VBOTEX);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 2 * 3, NULL, GL_DYNAMIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
    //glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOTEX);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glUseProgram(shaderProgramTex);
    glBindBuffer(GL_ARRAY_BUFFER, VBOTEX);
    glDrawElements(GL_TRIANGLES, 48, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    //glDrawArrays(GL_TRIANGLES, 0, 2);
    
    kmVec3 color;
    color.x = 0.0f;
    color.y = 0.0f;
    color.z = 0.0f;
    renderText("0", 453.0f, 287.0f, 0.35f, color);
    renderText("50", 578.0f, 287.0f, 0.35f, color);
    renderText("99", 710.0f, 287.0f, 0.35f, color);
    renderText("Head :", 465.0f, 50.0f, 0.7f, color);
    char headText[10];
    itoa(head, headText);
    renderText(headText, 580.0f, 50.0f, 0.7f, color);
    
    float texPos = 480.0f;
    for (int i = 0; i < 5; i++)
    {
        char text[10];
        itoa(queueList[i], text);
        renderText(text, texPos, 330.0f, 0.45f, color);
        texPos += 50.0f;
    }

    for(int i=0; i<5; i++)
    {
        queue1[i] = abs(head-queueList[i]);
    }

    for(int i=0; i<5; i++)
    {
        for(int j=i+1; j<5;j++)
        {
            if(queue1[i]>queue1[j])
            {
                temp = queue1[i];  /*SORTING*/
                queue1[i]=queueList[j];
                queue1[j]=temp;
                temp=queueList[i];
                queueList[i]=queueList[j];
                queueList[j]=temp;
            }
        }
    }
    
    float xOffset = 0.14f;
    float mult = 0.0068f;
    xPos1 = xOffset + head * mult;
    yPos1 = 0.15f;
    float yOffset = 0.10f;

    float vert [36] = {
            xPos1, yPos1, 0.0f, 1.0f, 1.0f, 1.0f,
        };

    for (int i=0; i<5; i++)
    {
        xPos1 = xOffset + queueList[i] * mult;
        yPos1 = yPos1 - 0.10f;

        vert[i+6+5*i] = xPos1;
        vert[i+7+5*i] = yPos1;
        vert[i+8+5*i] = 0.0f;
        vert[i+9+5*i] = 1.0f;
        vert[i+10+5*i] = 1.0f;
        vert[i+11+5*i] = 1.0f;

        //printf("%f\t%f\n", xPos1, yPos1);
        //xPos1 = xPos2;
        //yPos1 = yPos2;
        //head = queueList[i];
        //sleep(1);
    }

    glGenVertexArrays(1, &VAOVERT);
    glGenBuffers(1, &VBOVERT);
    glBindVertexArray(VAOVERT);
    glBindBuffer(GL_ARRAY_BUFFER, VBOVERT);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vert), vert, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    glUseProgram(shaderProgramTex);

    glDrawArrays(GL_LINE_STRIP, 0, 6);
    
}

void diskLook()
{
    glUseProgram(shaderProgramTex);

    //kmMat4 projection;
    //kmMat4OrthographicProjection(&projection, 0.0f, 800.0f, 0.0f, 450.0f, 0.0f, 100.0f);
    //float *projPointer = valPtr(&projection);
    //glUniformMatrix4fv(glGetUniformLocation(shaderProgramText, "projection"), 1, GL_FALSE, projPointer);

    float rColor[5], gColor[5], bColor[5];

    for (int i=0; i<5; i++)
    {
        rColor[i] = 0.0f;
        gColor[i] = 0.0f;
        bColor[i] = 1.0f;
    }

    float vertices [] = {
        //first
        0.14f, -0.75f, 0.00f,   1.0f, 1.0f, 1.0f,
        0.14f,  0.25f, 0.00f,   1.0f, 1.0f, 1.0f,
        0.15f,  0.25f, 0.00f,   1.0f, 1.0f, 1.0f,
        0.15f, -0.75f, 0.00f,   1.0f, 1.0f, 1.0f,
        //second
        0.14f,  0.26f, 0.00f,   1.0f, 1.0f, 1.0f,
        0.82f,  0.26f, 0.00f,   1.0f, 1.0f, 1.0f,
        0.82f,  0.25f, 0.00f,   1.0f, 1.0f, 1.0f,
        0.14f,  0.25f, 0.00f,   1.0f, 1.0f, 1.0f,
        //third
        0.82f,  0.26f, 0.00f,   1.0f, 1.0f, 1.0f,
        0.83f,  0.26f, 0.00f,   1.0f, 1.0f, 1.0f,
        0.83f, -0.75f, 0.00f,   1.0f, 1.0f, 1.0f,
        0.82f, -0.75f, 0.00f,   1.0f, 1.0f, 1.0f,
        //I/O-1
        0.19f,  0.55f, 0.00f,   rColor[0], gColor[0], bColor[0],
        0.28f,  0.55f, 0.00f,   rColor[0], gColor[0], bColor[0],
        0.28f,  0.45f, 0.00f,   rColor[0], gColor[0], bColor[0],
        0.19f,  0.45f, 0.00f,   rColor[0], gColor[0], bColor[0],
        //I/O-2
        0.31f,  0.55f, 0.00f,   rColor[1], gColor[1], bColor[1],
        0.40f,  0.55f, 0.00f,   rColor[1], gColor[1], bColor[1],
        0.40f,  0.45f, 0.00f,   rColor[1], gColor[1], bColor[1],
        0.31f,  0.45f, 0.00f,   rColor[1], gColor[1], bColor[1],
        //I/O-3
        0.43f,  0.55f, 0.00f,   rColor[2], gColor[2], bColor[2],
        0.52f,  0.55f, 0.00f,   rColor[2], gColor[2], bColor[2],
        0.52f,  0.45f, 0.00f,   rColor[2], gColor[2], bColor[2],
        0.43f,  0.45f, 0.00f,   rColor[2], gColor[2], bColor[2],
        //I/O-4
        0.55f,  0.55f, 0.00f,   rColor[3], gColor[3], bColor[3],
        0.65f,  0.55f, 0.00f,   rColor[3], gColor[3], bColor[3],
        0.65f,  0.45f, 0.00f,   rColor[3], gColor[3], bColor[3],
        0.55f,  0.45f, 0.00f,   rColor[3], gColor[3], bColor[3],
        //I/O-5
        0.68f,  0.55f, 0.00f,   rColor[4], gColor[4], bColor[4],
        0.77f,  0.55f, 0.00f,   rColor[4], gColor[4], bColor[4],
        0.77f,  0.45f, 0.00f,   rColor[4], gColor[4], bColor[4],
        0.68f,  0.45f, 0.00f,   rColor[4], gColor[4], bColor[4]
    };

    unsigned int indices [] = {
        //first
        0, 1, 3,
        1, 2, 3,
        //second
        4, 5, 6,
        4, 6, 7,
        //third
        8, 9, 10,
        8, 10, 11,
        //I/O-1
        12, 13, 14,
        12, 14, 15,
        //I/O-2
        16, 17, 18,
        16, 18, 19,
        //I/O-3
        20, 21, 22,
        20, 22, 23,
        //I/O-4
        24, 25, 26,
        24, 26, 27,
        //I/O-5
        28, 29, 30,
        28, 30, 31
    };

    // Configure VAO/VBO for texture quads
    glGenVertexArrays(1, &VAOTEX);
    glGenBuffers(1, &VBOTEX);
    glGenBuffers(1, &EBOTEX);

    glBindVertexArray(VAOTEX);
    glBindBuffer(GL_ARRAY_BUFFER, VBOTEX);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 2 * 3, NULL, GL_DYNAMIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
    //glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOTEX);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glUseProgram(shaderProgramTex);
    glBindBuffer(GL_ARRAY_BUFFER, VBOTEX);
    glDrawElements(GL_TRIANGLES, 48, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    //glDrawArrays(GL_TRIANGLES, 0, 2);
    
    kmVec3 color;
    color.x = 0.0f;
    color.y = 0.0f;
    color.z = 0.0f;
    renderText("0", 453.0f, 287.0f, 0.35f, color);
    renderText("50", 578.0f, 287.0f, 0.35f, color);
    renderText("99", 710.0f, 287.0f, 0.35f, color);
    renderText("Head :", 465.0f, 50.0f, 0.7f, color);
    char headText[10];
    itoa(head, headText);
    renderText(headText, 580.0f, 50.0f, 0.7f, color);
    
    float texPos = 480.0f;
    for (int i = 0; i < 5; i++)
    {
        char text[10];
        itoa(queueList[i], text);
        renderText(text, texPos, 330.0f, 0.45f, color);
        texPos += 50.0f;
    }

    for (int i = 0; i < 5; i++)
    {
        queueList[i] = queueLook[i];
    }
    
    float xOffset = 0.14f;
    float mult = 0.0068f;
    xPos1 = xOffset + head * mult;
    yPos1 = 0.15f;
    float yOffset = 0.10f;

    float vert [36] = {
            xPos1, yPos1, 0.0f, 1.0f, 1.0f, 1.0f,
        };

    for (int i=0; i<5; i++)
    {
        xPos1 = xOffset + queueList[i] * mult;
        yPos1 = yPos1 - 0.10f;

        vert[i+6+5*i] = xPos1;
        vert[i+7+5*i] = yPos1;
        vert[i+8+5*i] = 0.0f;
        vert[i+9+5*i] = 1.0f;
        vert[i+10+5*i] = 1.0f;
        vert[i+11+5*i] = 1.0f;
    }

    glGenVertexArrays(1, &VAOVERT);
    glGenBuffers(1, &VBOVERT);
    glBindVertexArray(VAOVERT);
    glBindBuffer(GL_ARRAY_BUFFER, VBOVERT);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vert), vert, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    glUseProgram(shaderProgramTex);

    glDrawArrays(GL_LINE_STRIP, 0, 6);
    
}

void diskScan()
{
    glUseProgram(shaderProgramTex);

    //kmMat4 projection;
    //kmMat4OrthographicProjection(&projection, 0.0f, 800.0f, 0.0f, 450.0f, 0.0f, 100.0f);
    //float *projPointer = valPtr(&projection);
    //glUniformMatrix4fv(glGetUniformLocation(shaderProgramText, "projection"), 1, GL_FALSE, projPointer);

    float rColor[5], gColor[5], bColor[5];

    for (int i=0; i<5; i++)
    {
        rColor[i] = 0.0f;
        gColor[i] = 0.0f;
        bColor[i] = 1.0f;
    }

    float vertices [] = {
        //first
        0.14f, -0.75f, 0.00f,   1.0f, 1.0f, 1.0f,
        0.14f,  0.25f, 0.00f,   1.0f, 1.0f, 1.0f,
        0.15f,  0.25f, 0.00f,   1.0f, 1.0f, 1.0f,
        0.15f, -0.75f, 0.00f,   1.0f, 1.0f, 1.0f,
        //second
        0.14f,  0.26f, 0.00f,   1.0f, 1.0f, 1.0f,
        0.82f,  0.26f, 0.00f,   1.0f, 1.0f, 1.0f,
        0.82f,  0.25f, 0.00f,   1.0f, 1.0f, 1.0f,
        0.14f,  0.25f, 0.00f,   1.0f, 1.0f, 1.0f,
        //third
        0.82f,  0.26f, 0.00f,   1.0f, 1.0f, 1.0f,
        0.83f,  0.26f, 0.00f,   1.0f, 1.0f, 1.0f,
        0.83f, -0.75f, 0.00f,   1.0f, 1.0f, 1.0f,
        0.82f, -0.75f, 0.00f,   1.0f, 1.0f, 1.0f,
        //I/O-1
        0.19f,  0.55f, 0.00f,   rColor[0], gColor[0], bColor[0],
        0.28f,  0.55f, 0.00f,   rColor[0], gColor[0], bColor[0],
        0.28f,  0.45f, 0.00f,   rColor[0], gColor[0], bColor[0],
        0.19f,  0.45f, 0.00f,   rColor[0], gColor[0], bColor[0],
        //I/O-2
        0.31f,  0.55f, 0.00f,   rColor[1], gColor[1], bColor[1],
        0.40f,  0.55f, 0.00f,   rColor[1], gColor[1], bColor[1],
        0.40f,  0.45f, 0.00f,   rColor[1], gColor[1], bColor[1],
        0.31f,  0.45f, 0.00f,   rColor[1], gColor[1], bColor[1],
        //I/O-3
        0.43f,  0.55f, 0.00f,   rColor[2], gColor[2], bColor[2],
        0.52f,  0.55f, 0.00f,   rColor[2], gColor[2], bColor[2],
        0.52f,  0.45f, 0.00f,   rColor[2], gColor[2], bColor[2],
        0.43f,  0.45f, 0.00f,   rColor[2], gColor[2], bColor[2],
        //I/O-4
        0.55f,  0.55f, 0.00f,   rColor[3], gColor[3], bColor[3],
        0.65f,  0.55f, 0.00f,   rColor[3], gColor[3], bColor[3],
        0.65f,  0.45f, 0.00f,   rColor[3], gColor[3], bColor[3],
        0.55f,  0.45f, 0.00f,   rColor[3], gColor[3], bColor[3],
        //I/O-5
        0.68f,  0.55f, 0.00f,   rColor[4], gColor[4], bColor[4],
        0.77f,  0.55f, 0.00f,   rColor[4], gColor[4], bColor[4],
        0.77f,  0.45f, 0.00f,   rColor[4], gColor[4], bColor[4],
        0.68f,  0.45f, 0.00f,   rColor[4], gColor[4], bColor[4]
    };

    unsigned int indices [] = {
        //first
        0, 1, 3,
        1, 2, 3,
        //second
        4, 5, 6,
        4, 6, 7,
        //third
        8, 9, 10,
        8, 10, 11,
        //I/O-1
        12, 13, 14,
        12, 14, 15,
        //I/O-2
        16, 17, 18,
        16, 18, 19,
        //I/O-3
        20, 21, 22,
        20, 22, 23,
        //I/O-4
        24, 25, 26,
        24, 26, 27,
        //I/O-5
        28, 29, 30,
        28, 30, 31
    };

    // Configure VAO/VBO for texture quads
    glGenVertexArrays(1, &VAOTEX);
    glGenBuffers(1, &VBOTEX);
    glGenBuffers(1, &EBOTEX);

    glBindVertexArray(VAOTEX);
    glBindBuffer(GL_ARRAY_BUFFER, VBOTEX);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 2 * 3, NULL, GL_DYNAMIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
    //glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOTEX);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glUseProgram(shaderProgramTex);
    glBindBuffer(GL_ARRAY_BUFFER, VBOTEX);
    glDrawElements(GL_TRIANGLES, 48, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    //glDrawArrays(GL_TRIANGLES, 0, 2);
    
    kmVec3 color;
    color.x = 0.0f;
    color.y = 0.0f;
    color.z = 0.0f;
    renderText("0", 453.0f, 287.0f, 0.35f, color);
    renderText("50", 578.0f, 287.0f, 0.35f, color);
    renderText("99", 710.0f, 287.0f, 0.35f, color);
    renderText("Head :", 465.0f, 50.0f, 0.7f, color);
    char headText[10];
    itoa(head, headText);
    renderText(headText, 580.0f, 50.0f, 0.7f, color);
    
    float texPos = 480.0f;
    for (int i = 0; i < 5; i++)
    {
        char text[10];
        itoa(queueList[i], text);
        renderText(text, texPos, 330.0f, 0.45f, color);
        texPos += 50.0f;
    }
    
    float xOffset = 0.14f;
    float mult = 0.0068f;
    xPos1 = xOffset + head * mult;
    yPos1 = 0.15f;
    float yOffset = 0.10f;

    for (int i = 0; i < 6; i++)
    {
        queueList[i] = queueScan[i];
    }

    float vert [36] = {
            xPos1, yPos1, 0.0f, 1.0f, 1.0f, 1.0f,
        };

    for (int i=0; i<6; i++)
    {
        xPos1 = xOffset + queueList[i] * mult;
        yPos1 = yPos1 - 0.10f;

        vert[i+6+5*i] = xPos1;
        vert[i+7+5*i] = yPos1;
        vert[i+8+5*i] = 0.0f;
        vert[i+9+5*i] = 1.0f;
        vert[i+10+5*i] = 1.0f;
        vert[i+11+5*i] = 1.0f;
    }

    glGenVertexArrays(1, &VAOVERT);
    glGenBuffers(1, &VBOVERT);
    glBindVertexArray(VAOVERT);
    glBindBuffer(GL_ARRAY_BUFFER, VBOVERT);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vert), vert, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    glUseProgram(shaderProgramTex);

    glDrawArrays(GL_LINE_STRIP, 0, 6);
    
}