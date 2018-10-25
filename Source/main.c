#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#include "Resources/Headers/stb_image.h"

#include "firstscreen.h"

void processInput(GLFWwindow*);
//void set_framebuffer_size_callback(GLFWwindow*, int, int);
//void mouse_callback(GLFWwindow*, double, double);

//Window Setting
const int WIDTH = 1366, HEIGHT = 768;
int state = 0;

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
    GLFWwindow *window = glfwCreateWindow(WIDTH,HEIGHT,"Visual representation of Algos",NULL,NULL);
    if(window == NULL)
    {
        printf("Failed to create Window");
        glfwTerminate();
        return -1;
    }

    //Setting the Current Window to the one we created
    glfwMakeContextCurrent(window);
    //glfwSetFramebufferSizeCallback(window,set_framebuffer_size_callback);
    //glfwSetCursorPosCallback(window, mouse_callback);


    //Loading GLAD and other OpenGL Libs 
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("Failed to load GLAD");
        return -1;
    }
    compileShader();
    initBg();
    //initOpt3();

    while(!glfwWindowShouldClose(window))
    {
        
        //glClearColor(0.4f, 0.3f, 0.6f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        processInput(window);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    switch(state)
    {
        case 0:
            back();
            break;
        case 1:
            opt3a();
            break;
    }
}