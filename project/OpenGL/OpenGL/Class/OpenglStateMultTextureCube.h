#pragma once
#include <iostream>
#include "openglstate.h"
class OpenglStateMultTextureCube :
	public OpenglState
{
public:
	OpenglStateMultTextureCube();
	bool init(string vertFile, string fragFile);
	bool isUseEBORender();
	void rendeCommand();
	int getShaderIndex();
	void enableVertexAttribArray();
};

