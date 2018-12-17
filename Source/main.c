#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define STB_IMAGE_IMPLEMENTATION

#include "firstscreen.h"
#include "rendertext.h"
#include "pagerep.h"
#include "disksch.h"
#include "pagerepFifo.h"

void processInput(GLFWwindow*);
void set_framebuffer_size_callback(GLFWwindow*, int, int);
void mouse_callback(GLFWwindow*, int, int, int);
void cursor_callback(GLFWwindow*, double, double);
void get_resolution(int* window_width, int* window_height);

//Window Setting
const int WIDTH = 800, HEIGHT = 450;
int state = 0; //0 for main screen, 1 for DS, 2 for PF, 3 for Frag/Defrag
int diskSwitch = 0;
int pageSwitch = 0;
int fragSwitch = 0;

double lastX, lastY;

int main()
{
    //GLFW initialisation
    glfwInit();
    //Window Properties
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE,GL_TRUE);

    //Creating a Window
    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Visual representation of Algos", NULL, NULL);
    if(window == NULL)
    {
        printf("Failed to create Window");
        glfwTerminate();
        return -1;
    }

    //Setting the Current Window to the one we created
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window,set_framebuffer_size_callback);
    glfwSetMouseButtonCallback(window, mouse_callback);
    glfwSetCursorPosCallback(window, cursor_callback);

    //Loading GLAD and other OpenGL Libs 
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("Failed to load GLAD");
        return -1;
    }
    compileShader();
    initBg();
    initDisk();
    initFcfs();
    initScan();
    initSstf();
    initLook();
    initTextGlyphs();
    compileShaderDiskAlgo();

    //glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    while(!glfwWindowShouldClose(window))
    {
        
        //glClearColor(0.4f, 0.3f, 0.6f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        processInput(window);
        
        glfwPollEvents();

        switch(state)
        {
            case 0:
                back();
                break;
            case 1:
                diskScreen();
                break;
            case 2:
                glfwSetWindowShouldClose(window, GL_TRUE);
            default :
                back();
                break;
        }

        switch(diskSwitch)
        {
            case 1:
                dispFcfs();
                break;
            case 2:
                dispSstf();
                break;
            case 3:
                dispLook();
                break;
            case 4:
                dispScan();
                break;
        }

        glfwSwapBuffers(window);
    }
    if(state == 2)
    {
        displayPageFifo();       
    }
    
}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    if(glfwGetKey(window, GLFW_KEY_BACKSPACE) == GLFW_PRESS)
    {
        if(diskSwitch!=0)
        {
            diskSwitch = 0;
            state = 1;
        }
    }
    if(glfwGetKey(window, GLFW_KEY_DELETE) == GLFW_PRESS)
    {
        state = 0;
        diskSwitch = 0;
    }
}

void set_framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, int button, int action, int mod )
{
    
    if (state == 0)
    {
        if(lastX < 0.50 && lastX > -0.50)
        {
            if(lastY < 0.15 && lastY > -0.10)
            {
                state = 1;
            }
            else if(lastY < -0.25 && lastY > -0.50)
            {
                state = 2;
            }
            else if(lastY < -0.65 && lastY > -0.90)
            {
                //state = 3;
            }
        }
    }
    
    if (state == 1)
    {
        if(lastX < 0.75 && lastX > 0.10)
        {
            if(lastY < 0.90 && lastY > 0.20)
            {
                diskSwitch = 1; //fcfs
                state = 6;
            }
        }
        if(lastX < -0.10 && lastX > -0.75)
        {
            if(lastY < 0.90 && lastY > 0.20)
            {
                diskSwitch = 2; //sstf
                state = 6;
            }
        }
        if(lastX < -0.10 && lastX > -0.75)
        {
            if(lastY < -0.20 && lastY > -0.90)
            {
                diskSwitch = 3; //look
                state = 6;
            }
        }
        if(lastX < 0.75 && lastX > 0.10)
        {
            if(lastY < -0.20 && lastY > -0.90)
            {
                diskSwitch = 4; //scan
                state = 6;
            }
        }
    }
}

void cursor_callback(GLFWwindow *window, double xPos, double yPos)
{
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    lastX = -1.0 + 2.0 * xPos / width;
    lastY =  1.0 - 2.0 * yPos / height;
}

void get_resolution(int* window_width, int* window_height) {
    const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    *window_width = mode->width;
    *window_height = mode->height;
}
