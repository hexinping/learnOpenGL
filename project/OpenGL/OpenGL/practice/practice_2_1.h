#pragma once
#include "OpenglState.h"


/*
	des: 添加更多顶点到数据中，使用glDrawArrays，尝试绘制两个彼此相连的三角形
*/

class practice_2_1 :
	public OpenglState
{
public:
	practice_2_1();
	bool init(string vertFile, string fragFile);
	bool isUseEBORender();
	virtual void rendeCommand();
};

