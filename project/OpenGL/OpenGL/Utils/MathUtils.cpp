#include "MathUtils.h"

//glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);//��4��ֵĬ������Ϊ1
// ��ע��������Ǿ����ʼ����һ�����ӣ����ʹ�õ���0.9.9�����ϰ汾
// �������д������Ҫ��Ϊ:
// glm::mat4 trans = glm::mat4(1.0f)
// ֮�󽫲��ٽ�����ʾ

static MathUtils *instance = nullptr;

MathUtils * MathUtils::getInstance()
{
	if (!instance)
	{
		instance = new (std::nothrow) MathUtils();
	}
	return instance;
}

void MathUtils::translate(glm::vec4 *vec, glm::vec3 *transform)
{
	glm::mat4 trans; // Ĭ������һ����λ����
	trans = glm::translate(trans, *transform);
	*vec = trans * (*vec);
}


void MathUtils::rotate(glm::vec4 *vec, float angle, glm::vec3 *axis, bool isRaduis)
{
	glm::mat4 trans;
	if (!isRaduis)
	{
		trans = glm::rotate(trans, glm::radians(angle), *axis);
	}
	else
	{
		trans = glm::rotate(trans, angle, *axis);
	}
	
	*vec = trans * (*vec);
}

void MathUtils::scale(glm::vec4 *vec, glm::vec3 *s)
{
	glm::mat4 trans;
	trans = glm::scale(trans, *s);
	*vec = trans * (*vec);
}

void MathUtils::setTranslateMat4(glm::mat4 *mat, glm::vec3 *transform)
{
	*mat = glm::translate(*mat, *transform);
}


void MathUtils::setRotateMat4(glm::mat4 *mat, float angle, glm::vec3 *axis, bool isRaduis)
{
	if (!isRaduis)
	{
		*mat = glm::rotate(*mat, glm::radians(angle), *axis);
	}
	else
	{
		*mat = glm::rotate(*mat, angle, *axis);
	}
}

void MathUtils::setSclaeMat4(glm::mat4 *mat, glm::vec3 *s)
{
	*mat = glm::scale(*mat, *s);
}
