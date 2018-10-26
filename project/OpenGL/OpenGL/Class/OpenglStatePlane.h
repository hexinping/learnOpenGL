#pragma once
#include <iostream>
#include "openglstate.h"
class OpenglStatePlane :
	public OpenglState
{
public:
	OpenglStatePlane();
	bool init(string vertFile, string fragFile);
	bool isUseEBORender();
	void rendeCommand();
	int getShaderIndex();
	void enableVertexAttribArray();
};

