#pragma once
#include <iostream>
#include "openglstate.h"
class OpenglStateMultTextureCameraManuel :
	public OpenglState
{
public:
	OpenglStateMultTextureCameraManuel();
	bool init(string vertFile, string fragFile);
	bool isUseEBORender();
	void rendeCommand();
	int getShaderIndex();
	void enableVertexAttribArray();
};

