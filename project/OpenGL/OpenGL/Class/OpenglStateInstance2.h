#pragma once
#include <iostream>
#include "openglstate.h"
class OpenglStateInstance2 :
	public OpenglState
{
public:
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
	unsigned int instanceVBO;
};

