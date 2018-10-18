#pragma once
#include <iostream>
#include "openglstate.h"
class OpenglStateMultAttr :
	public OpenglState
{
public:
	OpenglStateMultAttr();
	bool init(string vertFile, string fragFile);
	bool isUseEBORender();
	void rendeCommand();
	int getShaderIndex();
	void enableVertexAttribArray();
};

