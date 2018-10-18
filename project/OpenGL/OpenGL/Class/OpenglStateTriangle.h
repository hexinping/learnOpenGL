#pragma once
#include <iostream>
#include "openglstate.h"
class OpenglStateTriangle :
	public OpenglState
{
public:
	OpenglStateTriangle();
	bool init(string vertFile, string fragFile);
	bool isUseEBORender();
	void rendeCommand();
	int getShaderIndex();
};

