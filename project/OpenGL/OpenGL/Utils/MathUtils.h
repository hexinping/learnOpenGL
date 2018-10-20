#pragma once
#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

using namespace std;
class MathUtils
{
public:
	static MathUtils * getInstance();

	//位移一个向量
	void translate(glm::vec4 *vec, glm::vec3 *transform);

	//旋转：角度，旋转轴
	void rotate(glm::vec4 *vec, float angle, glm::vec3 *axis, bool isRaduis = false);

	//缩放
	void scale(glm::vec4 *vec, glm::vec3 *s);

	//位移变化矩阵
	void setTranslateMat4(glm::mat4 *mat, glm::vec3 *transform);

	//旋转变化矩阵
	void setRotateMat4(glm::mat4 *mat, float angle, glm::vec3 *axis, bool isRaduis = false);

	//缩放变化矩阵
	void setSclaeMat4(glm::mat4 *mat, glm::vec3 *s);


	void setPerspective(glm::mat4 *mat , float radian,float width, float height, float nearD, float farD);
	

};

