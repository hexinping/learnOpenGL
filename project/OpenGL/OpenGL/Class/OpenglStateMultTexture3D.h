#pragma once
#include <iostream>
#include "openglstate.h"
class OpenglStateMultTexture3D :
	public OpenglState
{
public:
	OpenglStateMultTexture3D();
	bool init(string vertFile, string fragFile);
	bool isUseEBORender();
	void rendeCommand();
	int getShaderIndex();
	void enableVertexAttribArray();
};

