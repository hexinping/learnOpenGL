#pragma once
#include <iostream>
#include "openglstate.h"
class OpenglStateMultTexture :
	public OpenglState
{
public:
	bool init(string vertFile, string fragFile);
	bool isUseEBORender();
	void rendeCommand();
	int getShaderIndex();
	void enableVertexAttribArray();
};

