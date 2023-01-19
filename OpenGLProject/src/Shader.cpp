#include "Shader.h"
#include <glad/glad.h>
#include <fstream>
#include "functions.h"
#include <sstream>

Shader::Shader(const char *vertexsourcepath, const char *fragmentsourcepath)
{
	std::ifstream vertexsourcefile;
    std::stringstream vertexstringstream;
	std::string vertexcodestr;
    const char* vertexcode;

	std::ifstream fragmentsourcefile;
    std::stringstream fragmentstringstream;
	std::string fragmentcodestr;
    const char* fragmentcode;

	vertexsourcefile.open(vertexsourcepath);
	if(vertexsourcefile.is_open())
	{
        vertexstringstream << vertexsourcefile.rdbuf();
	}
    vertexcodestr = vertexstringstream.str();
    vertexcode = vertexcodestr.c_str();
	vertexsourcefile.close();

	fragmentsourcefile.open(fragmentsourcepath);
	if (fragmentsourcefile.is_open())
	{
        fragmentstringstream << fragmentsourcefile.rdbuf();
	}
    fragmentcodestr = fragmentstringstream.str();
    fragmentcode = fragmentcodestr.c_str();
	fragmentsourcefile.close();

    //shader id's
	unsigned int vertexshader, fragmentshader;

    //debug information
    int shadercompilesuccess;
    char infolog[512];

    //create and compile vertex shader
    vertexshader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexshader, 1, &vertexcode, nullptr);
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
    glShaderSource(fragmentshader, 1, &fragmentcode, nullptr);
    glCompileShader(fragmentshader);

    //check for compile errors
    glGetShaderiv(fragmentshader, GL_COMPILE_STATUS, &shadercompilesuccess);
    if (!shadercompilesuccess)
    {
        glGetShaderInfoLog(fragmentshader, 512, nullptr, &infolog[0]);
        printErr("Error compiling fragment shader");
        printErr(infolog);
    }

    //create program to link shaders
    _programId = glCreateProgram();
    glAttachShader(_programId, vertexshader);
    glAttachShader(_programId, fragmentshader);
    glLinkProgram(_programId);

    //shaders can be safely deleted after linking
    glDeleteShader(vertexshader);
    glDeleteShader(fragmentshader);
}

void Shader::useshaders()
{
    glUseProgram(_programId);
}