#include <glfw3.h>
#include <string>
#include <iostream>

//define constants for preferred window size
enum
{
	INIT_WINDOW_WIDTH = 700,
	INIT_WINDOW_HEIGHT = 500
};

void printErr(std::string message);
void getInput(GLFWwindow* win);