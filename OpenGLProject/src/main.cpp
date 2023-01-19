#include <glad/glad.h>
#include "functions.h"
#include "Shader.h"

//define constants for preferred window size
enum
{
    INIT_WINDOW_WIDTH = 700,
    INIT_WINDOW_HEIGHT = 500
};

int main()
{
    GLFWwindow* window;
    unsigned int vertexbufobj, vertarrobj, vertelemobj;

    //initialize glfw library
    if (!glfwInit())
    {
        printErr("An error occurred when initializing GLFW");
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
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printErr("An error occurred when initializing GLAD");
        return -1;
    }

    //load and compile shaders
    Shader myshaders("./shaders/vertex.vert", "./shaders/fragment.frag");

    //create vertices
    float vertices[] =
    {
        -0.5f, -0.5f, 0.0f,     //[2|6]+      [4]+
    	-0.5f,  0.5f, 0.0f,     //
         0.5f, -0.5f, 0.0f,     //
         0.5f,  0.5f, 0.0f      //  [1]+    [3|5]+
    };

    unsigned int indices[] =
    {
        0, 1, 2,                //indices of first triangle
        3, 2, 1                 //indices of second triangle
    };

    //create vertex array
    glGenVertexArrays(1, &vertarrobj);
    glBindVertexArray(vertarrobj);

    //create vertex buffer and copy vertex data into object
    glGenBuffers(1, &vertexbufobj);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbufobj);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &vertelemobj);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertelemobj);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

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
        myshaders.useshaders();
        glBindVertexArray(vertarrobj);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        //swap buffer and poll for events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
