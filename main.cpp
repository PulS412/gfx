#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>
#include "Shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>

// int width, height, nrChannels;
// unsigned char *data = stbi_load("oot_cow_box.png", &width, &height, &nrChannels, 0);

// unsigned int texture;
// glGenTextures(1, &texture) 
// glBindTexture(GL_TEXTURE_2D, texture);  

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void ProcessInput(GLFWwindow* window);
void CompileShaders(unsigned int * _shaderProgram, const char* _vertexShaderSource,const char* _fragmentShaderSource);

//window dimensions
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main() {
    //initialize the glfw library
    glfwInit();

    //window hints are settings that can be used to configure various
    //aspects of a window such as its size, position, and behavior before
    //creating the window and will affect how the window behaves.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //glfw window creation
    GLFWwindow* window = glfwCreateWindow(800, 600, "GLFW window", nullptr, nullptr);
    
    if(window == NULL){
        std::cout << "Failed to vreate GLFW window\n";
        glfwTerminate();
        return -1;
    }

    //Make the context of the window the main context of the current thread
    //to allow access and use of the context's data and state for rendering
    glfwMakeContextCurrent(window);

    //this sets the callback function that is called when the user resizes the window
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //Load all OpenGL function pointers to allow access to the functions defined in
    //the OpenGL library for rendering
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Failed to initialize glad\n";
        return -1;
    }

    float vertices_a[] = {
        //left triangle positions   //colors
        -1.0f, -1.0f, 0.0f,         1.0f, 0.0f, 0.0f, //bottom right     
        0.0f, -1.0f, 0.0f,          0.0f, 1.0f, 0.0f, //bottom left    
        -0.5f, 0.0f, 0.0f,          0.0f, 0.0f, 1.0f, //top   
        //top triangle positions    //colors
        0.5f, 0.0f, 0.0f,           1.0f, 0.0f, 0.0f, //bottom right       
        -0.5f, 0.0f, 0.0f,          0.0f, 1.0f, 0.0f, //bottom left
        0.0f, 1.0f, 0.0f,           0.0f, 0.0f, 1.0f, //top
    };

    float vertices_b[] = {
        //right triangle positions  //colors
        1.0f, -1.0f, 0.0f,          1.0f, 0.0f, 0.0f, //bottom right
        0.0f, -1.0f, 0.0f,          0.0f, 1.0f, 0.0f, //bottom left
        0.5f, 0.0f, 0.0f,           0.0f, 0.0f, 1.0f //top
    };

    float texCoords[]={
        0.0f, 0.0f,  // lower-left corner  
        1.0f, 0.0f,  // lower-right corner
        0.5f, 1.0f   // top-center corner    
    };

    float borderColor[]={1.0f, 1.0f, 0.0f, 1.0f};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    Shader ourShader("../shader.vs", "../shader.fs");
    //VBO can store a
    //large number of verticies in the GPUs memory
    unsigned int VBO[2];
    //VAO is a vertex array
    unsigned int VAO[2];
    // unsigned int EBO;

    //GenBuffers returns the names of the buffers via the pointer passed to it
    glGenVertexArrays(2, VAO);
    glGenBuffers(2, VBO);
    // glGenBuffers(1, &EBO);
    //bind the buffer to the gl array buffer target
    
    glBindVertexArray(VAO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    
    
    //copy the vertex data into the VBO buffer's memory
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_a), vertices_a, GL_STATIC_DRAW);
    //the buffer objexts that are to be used by the vertex stage of the GL
    //are collected together to form a vertex array object.

    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //set our vertex attributes pointers
    //position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    
    glBindVertexArray(VAO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_b), vertices_b, GL_STATIC_DRAW);
    //position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    //copy our vertices array in a buffer for opengl to use
    

    unsigned int shaderProgramYellow;
    // CompileShaders(&shaderProgramYellow, vertexShaderSource, fragmentShaderSourceYellow);
    // unsigned int shaderProgramBlue;
    // CompileShaders(&shaderProgramBlue, vertexShaderSource, fragmentShaderSourceBlue);
    //enable wireframe mode
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while(!glfwWindowShouldClose(window)) {
        ProcessInput(window);

        //rendering commands go here
        //...
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); //state setting function
        glClear(GL_COLOR_BUFFER_BIT); //state using function

        //draw the object

        float timeValue = glfwGetTime();
        float colorAmplitude = 3*sin(timeValue*2) + 3.0f;
        // int vertexColorLocation = glGetUniformLocation(shaderProgramYellow, "ourColor");
        // glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
        ourShader.setFloat("colorAmplitude", colorAmplitude);
        //render triangle
        ourShader.use();
        glBindVertexArray(VAO[0]);
        glDrawArrays(GL_TRIANGLES, 0, 12);

        // glUseProgram(shaderProgramBlue);
        glBindVertexArray(VAO[1]);
        glDrawArrays(GL_TRIANGLES, 0, 12);
        //render rectangle
        // glDrawElements(GL_TRIANGLE_STRIP, 12, GL_UNSIGNED_INT, 0);

        //swap buffers to prevent flickering that may arise as a result of
        //writing to the front buffer. Writing should always be done to the
        //back buffer while the front buffer is displayed
        glfwSwapBuffers(window);

        //check for any events that may be triggered
        //will update the window state and call callback functions
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

//query glfw whether relevant keys are pressed or released in this fream and react accordingly
void ProcessInput(GLFWwindow* window){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    
    //set the dimensions of the rendering area within the window
    //this sets up the transformation that maps the normalized
    //device coordinates of the rendered scene to the screen
    //coordinates of the viewport in pixels
    glViewport(0, 0, width, height);
}