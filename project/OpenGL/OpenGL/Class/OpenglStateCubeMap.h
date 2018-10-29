#pragma once
#include <iostream>
#include "openglstate.h"
class OpenglStateCubeMap :
	public OpenglState
{
public:
	OpenglStateCubeMap();
	bool init(string vertFile, string fragFile);
	bool isUseEBORender();
	void rendeCommand();
	int getShaderIndex();
	void enableVertexAttribArray();
	bool isUseSkyBox();
};

