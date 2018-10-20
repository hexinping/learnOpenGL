#include "MathUtils.h"

//glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);//第4个值默认设置为1
// 译注：下面就是矩阵初始化的一个例子，如果使用的是0.9.9及以上版本
// 下面这行代码就需要改为:
// glm::mat4 trans = glm::mat4(1.0f)
// 之后将不再进行提示

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
	glm::mat4 trans; // 默认生成一个单位矩阵
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
