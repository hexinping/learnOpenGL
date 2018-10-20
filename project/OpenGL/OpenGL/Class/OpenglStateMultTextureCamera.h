#pragma once
#include <iostream>
#include "openglstate.h"
class OpenglStateMultTextureCamera :
	public OpenglState
{
public:
	OpenglStateMultTextureCamera();
	bool init(string vertFile, string fragFile);
	bool isUseEBORender();
	void rendeCommand();
	int getShaderIndex();
	void enableVertexAttribArray();
};

