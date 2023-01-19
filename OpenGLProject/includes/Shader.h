#pragma once

class Shader
{
	unsigned int _programId;

	public:
		Shader(const char* vertexsourcepath, const char* fragmentsourcepath);

		void useshaders();
};