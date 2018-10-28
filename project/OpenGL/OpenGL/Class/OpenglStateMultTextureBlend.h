#pragma once
#include <iostream>
#include "openglstate.h"
class OpenglStateMultTextureBlend :
	public OpenglState
{
public:
	OpenglStateMultTextureBlend();
	bool init(string vertFile, string fragFile);
	bool isUseEBORender();
	void rendeCommand();
	int getShaderIndex();
	void enableVertexAttribArray();
	bool isLihgtAction();
	int getPointLights();
	bool isShowLight();
	bool  isUsePlane();

	unsigned int transparentVAO, transparentVBO;
};

