#pragma once
#include <iostream>
#include "openglstate.h"
class OpenglStateTexture :
	public OpenglState
{
public:
	OpenglStateTexture();
	bool init(string vertFile, string fragFile);
	bool isUseEBORender();
	void rendeCommand();
	int getShaderIndex();
	void enableVertexAttribArray();
};

