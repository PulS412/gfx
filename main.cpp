#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>

// This vertex shader takes in the 3D coordinates of an object's
//vertices and applies mathematical operations to transform them
//into 2D screen coordinates, which are then passed to the fragment
//shader to determine the color of each pixel on the screen
const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aColor;\n"
    
    "out vec3 ourColor;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos, 1.0);\n"
    "   ourColor = aColor;\n"
    "}\0";

// This is a simple fragment shader written in GLSL version 3.30
//that sets the final color of a fragment to a shade of orange
const char *fragmentShaderSourceYellow = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec3 ourColor;\n"
    "void main()\n"
    "{\n"
    "    FragColor = vec4(ourColor, 1.0);\n"
    "}\0";
// const char *fragmentShaderSourceBlue= "#version 330 core\n"
//     "out vec4 FragColor;\n"
//     "void main()\n"
//     "{\n"
//     "    FragColor = vec4(0.0f, 0.0f, 1.0f, 1.0f);\n"
//     "}\0";

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
    CompileShaders(&shaderProgramYellow, vertexShaderSource, fragmentShaderSourceYellow);
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
        glUseProgram(shaderProgramYellow);

        float timeValue = glfwGetTime();
        float greenValue = sin(timeValue) / 2.0f + 0.5f;
        int vertexColorLocation = glGetUniformLocation(shaderProgramYellow, "ourColor");
        glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
        //render triangle
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

void CompileShaders(unsigned int * _shaderProgram, const char* _vertexShaderSource, const char* _fragmentShaderSource){
    //compile shaders
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &_vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if(!success){
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &_fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    if(!success){
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    *_shaderProgram = glCreateProgram();
    glAttachShader(*_shaderProgram, vertexShader);
    glAttachShader(*_shaderProgram, fragmentShader);
    glLinkProgram(*_shaderProgram);
    
    // check for linking errors
    glGetProgramiv(*_shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(*_shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}