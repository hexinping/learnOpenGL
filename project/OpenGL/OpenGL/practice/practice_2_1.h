#pragma once
#include "OpenglState.h"


/*
	des: ��Ӹ��ඥ�㵽�����У�ʹ��glDrawArrays�����Ի��������˴�������������
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

