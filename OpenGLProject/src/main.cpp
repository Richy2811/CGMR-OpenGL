#include <glad/glad.h>
#include "functions.h"

int main()
{
    GLFWwindow* window;
    unsigned int vertexbufobj, vertarrobj, vertexshader, fragmentshader, shaderprogram;

    //GLSL code for vertex shader
    const char* vertexshaderstring =
        "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;"
        "void main()"
        "{"
			"gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);"
        "}";

    //glsl code for fragment shader
    const char* fragmentshaderstring =
        "#version 330 core\n"
        "out vec4 colour;"
        "void main()"
        "{"
			"colour = vec4(0.2f, 0.4f, 1.0f, 1.0f);"
        "}";

    //debug information
    int shadercompilesuccess = true;
    char infolog[512];

    //initialize glfw library
    if (!glfwInit())
    {
        printErr("An error occurred when initializing glfw");
	    return -1;
    }

    //set minimum OpenGL version requirement
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    //create window
    window = glfwCreateWindow(INIT_WINDOW_WIDTH, INIT_WINDOW_HEIGHT, "OpenGL Project Window", nullptr, nullptr);
    if (!window)
    {
        printErr("An error occurred when creating the window");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, [](GLFWwindow* win, int wid, int hei)
    {
    	glViewport(0, 0, wid, hei);
    });

    //initialize GLAD
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        printErr("An error occurred when initializing GLAD");
        return -1;
    }

    //create vertices
    float vertices[] =
    {
        -0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f,
         0.5f, -0.5f, 0.0f
    };

    //create and compile vertex shader
    vertexshader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexshader, 1, &vertexshaderstring, nullptr);
    glCompileShader(vertexshader);

    //check for compile errors
    glGetShaderiv(vertexshader, GL_COMPILE_STATUS, &shadercompilesuccess);
    if (!shadercompilesuccess)
    {
        glGetShaderInfoLog(vertexshader, 512, nullptr, &infolog[0]);
        printErr("Error compiling vertex shader");
        printErr(infolog);
    }

    //create and compile fragment shader
    fragmentshader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentshader, 1, &fragmentshaderstring, nullptr);
    glCompileShader(fragmentshader);

    //check for compile errors
    glGetShaderiv(fragmentshader, GL_COMPILE_STATUS, &shadercompilesuccess);
    if(!shadercompilesuccess)
    {
        glGetShaderInfoLog(fragmentshader, 512, nullptr, &infolog[0]);
        printErr("Error compiling fragment shader");
        printErr(infolog);
    }

    //create program to link shaders
    shaderprogram = glCreateProgram();
    glAttachShader(shaderprogram, vertexshader);
    glAttachShader(shaderprogram, fragmentshader);
    glLinkProgram(shaderprogram);

    //shaders can be safely deleted after linking
    glDeleteShader(vertexshader);
    glDeleteShader(fragmentshader);

    //create vertex array
    glGenVertexArrays(1, &vertarrobj);
    glBindVertexArray(vertarrobj);

    //copy vertex data into buffer
    glGenBuffers(1, &vertexbufobj);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbufobj);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //tell OpenGL how to link vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //loop until window is closed
    while (!glfwWindowShouldClose(window))
    {
        //clear frame
        glClearColor(0.7f, 0.7f, 0.7f, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        //get input
        getInput(window);

        //draw
        glUseProgram(shaderprogram);
        glBindVertexArray(vertarrobj);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        //swap buffer and poll for events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}