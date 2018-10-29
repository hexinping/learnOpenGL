#pragma once
#include <iostream>
#include "openglstate.h"
class OpenglStateReflect :
	public OpenglState
{
public:
	OpenglStateReflect();
	bool init(string vertFile, string fragFile);
	bool isUseEBORender();
	void rendeCommand();
	int getShaderIndex();
	void enableVertexAttribArray();
	bool isUseSkyBox();
};

