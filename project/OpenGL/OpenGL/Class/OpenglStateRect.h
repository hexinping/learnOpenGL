#pragma once
#include <iostream>
#include "openglstate.h"
class OpenglStateRect :
	public OpenglState
{
public:
	bool init(string vertFile, string fragFile);
	void rendeCommand();
	int getShaderIndex();
};

