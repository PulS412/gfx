#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>
#include "Shader.h"
#include "FileManager.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void ProcessInput(GLFWwindow* window);

//window dimensions
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main() {
    glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));
    vec = trans * vec;
    std::cout << vec.x << vec.y << vec.z << std::endl;


    FileManager cabinet;
    string vsShaderPath(cabinet.GetFilePath("shaders","shader.vs"));
    string fsShaderPath(cabinet.GetFilePath("shaders", "shader.fs"));
    string boxTexturePath(cabinet.GetFilePath("textures", "oot_cow_box.png"));
    string awesomeTexturePath(cabinet.GetFilePath("textures", "awesomeface.png"));

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

    float vertices[] = {
        // positions          // colors           // texture coords
        0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
        0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
    };
    unsigned int indices[]={
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle        
    };
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);  
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);  
    
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    unsigned int boxTexture, awesomeTexture;
    glGenTextures(1, &boxTexture);
    glBindTexture(GL_TEXTURE_2D, boxTexture);
    
    //set the texture wrapping/filtering options on the currently bound texture object
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER , GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER , GL_LINEAR);

    //get the current working directory for cross platform compatability (windows/linux)
    //load and generate the texture
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);  
    unsigned char *data = stbi_load(boxTexturePath.c_str(), &width, &height, &nrChannels, 0);
    if(data != nullptr){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else{
        std::cout << "Failed to load texture data" << std::endl;
        return 1;
    }
    stbi_image_free(data);

    glGenTextures(1, &awesomeTexture);
    glBindTexture(GL_TEXTURE_2D, awesomeTexture); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    data = stbi_load(awesomeTexturePath.c_str(), &width, &height, &nrChannels, 0);
    if(data != nullptr){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else{
        std::cout << "Failed to load texture data" << std::endl;
        return 1;
    }
    stbi_image_free(data);

    std::cout << "vertexPath" << vsShaderPath << std::endl;
    Shader ourShader(vsShaderPath.c_str(), fsShaderPath.c_str());
    // glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0); //set it manually
    ourShader.use();
    ourShader.setInt("texture2", 1); //or with shader class
    ourShader.setFloat("mixDegree", 0.8f);

    while(!glfwWindowShouldClose(window)) {
        ProcessInput(window);

        //rendering commands go here
        //...
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); //state setting function
        glClear(GL_COLOR_BUFFER_BIT); //state using function

        //draw the object
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, boxTexture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, awesomeTexture);
        //render
        
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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



