#include "OpenglStateModel3D.h"

OpenglStateModel3D::OpenglStateModel3D()
{
	OpenglState::OpenglState();
}

bool OpenglStateModel3D::init(string vertFile, string fragFile)
{
	_glUtils->createShaderWithFile(GL_VERTEX_SHADER, &_vertexShader, _vertFile);
	_glUtils->createShaderWithFile(GL_FRAGMENT_SHADER, &_fragmentShader, _fragFile);
	_glUtils->linkShader(&_shaderProgram, _vertexShader, _fragmentShader);
	return true;
}

int OpenglStateModel3D::getShaderIndex()
{
	return 17;
}


