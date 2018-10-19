#pragma once
#include <iostream>
#include "openglstate.h"
class OpenglStateMultTextureMat4 :
	public OpenglState
{
public:
	OpenglStateMultTextureMat4();
	bool init(string vertFile, string fragFile);
	bool isUseEBORender();
	void rendeCommand();
	int getShaderIndex();
	void enableVertexAttribArray();
};

