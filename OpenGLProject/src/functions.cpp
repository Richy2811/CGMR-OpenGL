#include "functions.h"

void printErr(std::string message)
{
    std::cerr << message << std::endl;
}

void getInput(GLFWwindow* win)
{
    //if spacebar is pressed
    if (glfwGetKey(win, GLFW_KEY_SPACE))
    {
        //set close window flag
        glfwSetWindowShouldClose(win, true);
    }
}