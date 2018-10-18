#pragma once
#include <iostream>
#include "openglstate.h"
class OpenglStateTriangleMove :
	public OpenglState
{
public:
	OpenglStateTriangleMove();
	bool init(string vertFile, string fragFile);
	bool isUseEBORender();
	void rendeCommand();
	int getShaderIndex();
	float offset = 0.0f;
};

