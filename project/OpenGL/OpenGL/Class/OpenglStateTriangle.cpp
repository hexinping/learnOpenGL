#include "OpenglStateTriangle.h"


OpenglStateTriangle::OpenglStateTriangle()
{
	OpenglState::OpenglState();
}

bool OpenglStateTriangle::init(string vertFile, string fragFile)
{
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f
	};
	memcpy(_vertices, vertices, sizeof(float) * 9);
	_vertFile = vertFile;
	_fragFile = fragFile;
	__super::initRendCommand();
	
	return true;
}
bool OpenglStateTriangle::isUseEBORender()
{
	return false;
}
