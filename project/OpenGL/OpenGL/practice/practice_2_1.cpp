#include "practice_2_1.h"

practice_2_1::practice_2_1()
{
	OpenglState::OpenglState();
}

bool practice_2_1::init(string vertFile, string fragFile)
{
	float vertices[] = {
		// first triangle
		-0.9f, -0.5f, 0.0f,  // left 
		-0.0f, -0.5f, 0.0f,  // right
		-0.45f, 0.5f, 0.0f,  // top 
		// second triangle
		0.0f, -0.5f, 0.0f,  // left
		0.9f, -0.5f, 0.0f,  // right
		0.45f, 0.5f, 0.0f   // top 
	};

	memcpy(_vertices, vertices, sizeof(float) * 18);

	_vertFile = vertFile;
	_fragFile = fragFile;
	this->initRendCommand();

	return true;
}
bool practice_2_1::isUseEBORender()
{
	return false;
}

void practice_2_1::rendeCommand()
{
	__super::rendeCommand();
	glDrawArrays(GL_TRIANGLES, 0, 6);
}
