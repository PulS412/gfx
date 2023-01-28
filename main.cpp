#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

// This vertex shader takes in the 3D coordinates of an object's
//vertices and applies mathematical operations to transform them
//into 2D screen coordinates, which are then passed to the fragment
//shader to determine the color of each pixel on the screen
const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

// This is a simple fragment shader written in GLSL version 3.30
//that sets the final color of a fragment to a shade of orange
const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\0";

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

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

    float verticies[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f
    };

    //create a vertex buffer object
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    //bind the buffer to the gl array buffer target
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //copy the vertex data into the buffer's memory
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);

    //create a vertex array object
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    //bind vertex array object
    glBindVertexArray(VAO);
    //copy our verticies array in a buffer for opengl to use
    glBindBuffer(GL_ARRAY_BUFFER, VAO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);
    //set our vertex attributes pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    //compile shaders
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
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
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    if(!success){
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    while(!glfwWindowShouldClose(window)) {
        processInput(window);

        //rengering commands go here
        //...
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); //state setting function
        glClear(GL_COLOR_BUFFER_BIT); //state using function

        //draw the object
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

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
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return 0;
}

//query glfw whether relevant keys are pressed or released in this fream and react accordingly
void processInput(GLFWwindow* window){
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