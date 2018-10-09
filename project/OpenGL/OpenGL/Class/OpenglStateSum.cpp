#include "OpenglStateSum.h"


OpenglStateSum::OpenglStateSum()
{
	OpenglState::OpenglState();
}

bool OpenglStateSum::init(string vertFile, string fragFile)
{
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f
	};
	memcpy(_vertices, vertices, sizeof(float) * 9);
	_vertFile = vertFile;
	_fragFile = fragFile;
	this->initRendCommand();
	return true;
}
bool OpenglStateSum::isUseEBORender()
{
	return false;
}
