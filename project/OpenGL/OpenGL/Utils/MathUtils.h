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

	//λ��һ������
	void translate(glm::vec4 *vec, glm::vec3 *transform);

	//��ת���Ƕȣ���ת��
	void rotate(glm::vec4 *vec, float angle, glm::vec3 *axis, bool isRaduis = false);

	//����
	void scale(glm::vec4 *vec, glm::vec3 *s);

	//λ�Ʊ仯����
	void setTranslateMat4(glm::mat4 *mat, glm::vec3 *transform);

	//��ת�仯����
	void setRotateMat4(glm::mat4 *mat, float angle, glm::vec3 *axis, bool isRaduis = false);

	//���ű仯����
	void setSclaeMat4(glm::mat4 *mat, glm::vec3 *s);


	void setPerspective(glm::mat4 *mat , float radian,float width, float height, float nearD, float farD);
	

};

