#include <glad/glad.h>
#include "Shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include <glfw3.h>
#include "stb_image.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "functions.h"

void getInput(GLFWwindow* win);

//define constants for preferred window size
enum
{
    INIT_WINDOW_WIDTH = 700,
    INIT_WINDOW_HEIGHT = 500
};

glm::vec3 camerapos = glm::vec3(0.0f, 0.0f, 5.0f);
glm::vec3 cameraforward = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraup = glm::vec3(0.0f, 1.0f, 0.0f);

bool firstMouse = true;
float yaw = -90.0f;
float pitch = 0.0f;
float lastX = (float)INIT_WINDOW_WIDTH / 2; //middle of view
float lastY = (float)INIT_WINDOW_HEIGHT / 2;  //middle of view

int main()
{
    GLFWwindow* window;
    unsigned int vertexbufobjcube, vertarrobjcube;
    unsigned int vertelemobjmiddle;
    unsigned int sampletexture;
    unsigned char* imagedata;
    int texturewidth, textureheight, colourchannels;
    float imagebordercolour[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    unsigned int model, view, projection;

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

    glEnable(GL_DEPTH_TEST);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, [](GLFWwindow* win, double xpos, double ypos)
    {
        if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

	    float xoffset = xpos - lastX;
	    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
	    lastX = xpos;
	    lastY = ypos;

	    float sensitivity = 0.1f; // change this value to your liking
	    xoffset *= sensitivity;
	    yoffset *= sensitivity;

	    yaw += xoffset;
	    pitch += yoffset;

	    // make sure that when pitch is out of bounds, screen doesn't get flipped
	    if (pitch > 89.0f)
	    {
		    pitch = 89.0f;
	    }
	    if (pitch < -89.0f)
	    {
		    pitch = -89.0f;
	    }

	    glm::vec3 front;
	    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	    front.y = sin(glm::radians(pitch));
	    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	    cameraforward = glm::normalize(front);
    });

    //load and compile shaders
    Shader myshaders("./shaders/vertex.vert", "./shaders/fragment.frag");

    //create vertices
    float vertices[] =
    {//vertex position data | texture position data
    	-0.5f, -0.5f, -0.5f,    0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,    1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,    1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,    1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,    0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,    0.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,    0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,    1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,    1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,    1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,    0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,    0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,    1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,    1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,    0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,    0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,    0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,    1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,    1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,    1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,    0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,    0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,    0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,    1.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,    0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,    1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,    1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,    1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,    0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,    0.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,    0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,    1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,    1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,    1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,    0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,    0.0f, 1.0f
    };

    //create and bind texture operations to current texture
    glGenTextures(1, &sampletexture);
    glBindTexture(GL_TEXTURE_2D, sampletexture);

    //set texture options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, imagebordercolour);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //copy image data
    imagedata = stbi_load("./textures/sampletexture.jpg", &texturewidth, &textureheight, &colourchannels, 0);

    if(imagedata)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texturewidth, textureheight, 0, GL_RGB, GL_UNSIGNED_BYTE, imagedata);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        printErr("Error loading texture");
    }

    //free allocated data
    stbi_image_free(imagedata);

    //create vertex array
    glGenVertexArrays(1, &vertarrobjcube);
    glBindVertexArray(vertarrobjcube);

    //create vertex buffer and copy vertex data into object
    glGenBuffers(1, &vertexbufobjcube);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbufobjcube);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //tell OpenGL where vertex position data can be found in vertices array
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //tell OpenGL where texture position data can be found in vertices array
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //loop until window is closed
    while (!glfwWindowShouldClose(window))
    {
        //clear frame
        glClearColor(0.7f, 0.7f, 0.7f, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //get input
        getInput(window);

        //bind texture
        glBindTexture(GL_TEXTURE_2D, sampletexture);

        //create rotation matrix
        glm::mat4 modeltransform = glm::mat4(1.0f);
        //scale model
        modeltransform = glm::scale(modeltransform, glm::vec3(0.4f, 0.4f, 0.4f));
        //rotate rectangle around 0, 0, 0
        modeltransform = glm::translate(modeltransform, glm::vec3(0, 0, -1.7f));
        modeltransform = glm::rotate(modeltransform, (float)glfwGetTime(), glm::vec3(0, 1, 0));
        modeltransform = glm::translate(modeltransform, glm::vec3(0, 0, 1.7f));

        //glm::mat4 modeltransform = glm::mat4(1.0f);
        //modeltransform = glm::rotate(modeltransform, glm::radians(-50.0f), glm::vec3(1, 0, 0));
        model = glGetUniformLocation(myshaders.getId(), "model");
        glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(modeltransform));

        glm::mat4 viewtransform = glm::mat4(1.0f);
        viewtransform = glm::lookAt(camerapos, camerapos + cameraforward, cameraup);
        view = glGetUniformLocation(myshaders.getId(), "view");
        glUniformMatrix4fv(view, 1, GL_FALSE, glm::value_ptr(viewtransform));

        glm::mat4 projectiontransform = glm::mat4(1.0f);
        projectiontransform = glm::perspective(glm::radians(45.0f), (float)INIT_WINDOW_HEIGHT / (float)INIT_WINDOW_WIDTH, 0.1f, 100.0f);
        projection = glGetUniformLocation(myshaders.getId(), "projection");
        glUniformMatrix4fv(projection, 1, GL_FALSE, glm::value_ptr(projectiontransform));

        //draw
        myshaders.useshaders();
        glBindVertexArray(vertarrobjcube);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //swap buffer and poll for events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void getInput(GLFWwindow* win)
{
    //if spacebar is pressed
    if (glfwGetKey(win, GLFW_KEY_SPACE))
    {
        //set close window flag
        glfwSetWindowShouldClose(win, true);
    }

    float cameraspeed = 0.04f;
    if (glfwGetKey(win, GLFW_KEY_W))
    {
        camerapos += cameraspeed * cameraforward;
    }
    if (glfwGetKey(win, GLFW_KEY_S))
    {
        camerapos -= cameraspeed * cameraforward;
    }
    if (glfwGetKey(win, GLFW_KEY_A))
    {
        camerapos -= glm::normalize(glm::cross(cameraforward, cameraup)) * cameraspeed;
    }
    if (glfwGetKey(win, GLFW_KEY_D))
    {
        camerapos += glm::normalize(glm::cross(cameraforward, cameraup)) * cameraspeed;
    }
}