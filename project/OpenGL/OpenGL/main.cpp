
#include "main.h"
#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <string>  
#include <fstream>  
#include <streambuf>  
using namespace std;


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

//const char *vertexShaderSource = "#version 330 core\n"
//"layout (location = 0) in vec3 aPos;\n"
//"void main()\n"
//"{\n"
//"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
//"}\0";
//
//const char *fragmentShaderSource = "#version 330 core\n"
//"out vec4 FragColor;\n"
//"void main()\n"
//"{\n"
//"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
//"}\n\0";
//

//.frag .vert�ļ���ȡ
char* readTheFile(string strSource) {
	//fstream ���ж��Ƿ�ɹ����ļ�
	//https://blog.csdn.net/fruitz/article/details/41624921
	std::ifstream myfile(strSource);
	//��ʧ��
	if (myfile.fail()) {
		std::cout << "Can not open it " << std::endl;
		throw new runtime_error("IO stream corrupted");
	}
	//ת�����ַ���
	std::string str((std::istreambuf_iterator<char>(myfile)),
		std::istreambuf_iterator<char>());
	//�ر��ļ�������
	myfile.close();
	//str���鳤��һ��Ҫ +1,
	/*ԭ��: https://blog.csdn.net/ShiQW5696/article/details/80676290 */
	int len = str.length();
	char* result = new char[len];
	strcpy_s(result, len + 1, str.c_str());
	return result;
}

void bindVBOAndVAO(unsigned int *VBO, unsigned int *VAO)
{
	// 1 �������ݴ���
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f
	};

	
	glGenVertexArrays(1, VAO); //����һ��vao��ֻҪ�Ȼ�һ�飬vao���¼���Ƶ����ԣ�Ȼ������ѭ���ﲻ���л�vao�ͺ�

	glBindVertexArray(*VAO); //�����Ȱ�VAO

	glGenBuffers(1, VBO); //����һ��VBO����:�൱�����Դ��Ͽ���һ��洢�ռ�

	glBindBuffer(GL_ARRAY_BUFFER, *VBO);//glBindBuffer�������´����Ļ���󶨵�GL_ARRAY_BUFFERĿ����

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);//�Ѷ���Ķ������ݸ��Ƶ�������ڴ��У�VBO��
}

void  createShaderWithSource(int shaderType, unsigned int *pShader, const char *shaderSource)
{
	unsigned int shader  ;
	shader = glCreateShader(shaderType); //����һ��������ɫ�������ݵĲ�����GL_VERTEX_SHADER��
	glShaderSource(shader, 1, &shaderSource, NULL); //��ɫ��Դ�븽�ӵ���ɫ��������
	glCompileShader(shader);//����

	//�ж��Ƿ����ɹ�
	int  success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);

		if (shaderType == GL_VERTEX_SHADER)
		{
			cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
		}
		else
		{
			cout << "ERROR::SHADER::VERTEX::LINK_FAILED\n" << infoLog << endl;
		}
		return;

	}
	*pShader = shader;
}

void  createShaderWithFile(int shaderType, unsigned int *pShader, string file)
{
	char * shaderSource = readTheFile(file);
	createShaderWithSource(shaderType, pShader, shaderSource);
}



int linkShader(unsigned int *shaderProgram, unsigned int vertexShader, unsigned int fragmentShader)
{
	//4������ɫ����������ɫ�������Ҽ���
	unsigned int program;
	program = glCreateProgram(); //������ɫ���������
	glAttachShader(program, vertexShader);  //���Ӷ�����ɫ��
	glAttachShader(program, fragmentShader);//����Ƭ����ɫ��
	glLinkProgram(program); // ��ʼ���Ӷ�����ɫ����Ƭ����ɫ��

	int  success;
	char infoLog[512];
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		cout << "ERROR::SHADER::VERTEX::LINK_FAILED\n" << infoLog << endl;
		return -1;
	}

	glDeleteShader(vertexShader); //����ɫ���������ӵ���������Ժ󣬼ǵ�ɾ����ɫ���������ǲ�����Ҫ������
	glDeleteShader(fragmentShader);
	*shaderProgram = program;
	return 0;
}


int main(int argc, char* argv[])
{
	glfwInit();

	// ����opegl�汾��Ϊ3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	//ͬ����ȷ����GLFW����ʹ�õ��Ǻ���ģʽ(Core-profile) ��ȷ����GLFW������Ҫʹ�ú���ģʽ��ζ������ֻ��ʹ��OpenGL���ܵ�һ���Ӽ�
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);


	int width = 800;
	int heght = 600;

	GLFWwindow* window = glfwCreateWindow(width, heght, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, width, heght);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// ------------------------------------------------------------------

	//1 �������ݴ���
	unsigned int VBO,VAO;
	bindVBOAndVAO(&VBO, &VAO);

	//2 ������ɫ��
	unsigned int vertexShader;
	//createShaderWithSource(GL_VERTEX_SHADER, &vertexShader, vertexShaderSource);
	createShaderWithFile(GL_VERTEX_SHADER, &vertexShader, "shader/base.vert");

	//3Ƭ����ɫ��
	unsigned int fragmentShader;
	//createShaderWithSource(GL_FRAGMENT_SHADER, &fragmentShader, fragmentShaderSource);
	createShaderWithFile(GL_FRAGMENT_SHADER, &fragmentShader, "shader/base.frag");


	//4������ɫ����������ɫ�������Ҽ���
	unsigned int shaderProgram;
	linkShader(&shaderProgram, vertexShader, fragmentShader);
	

	//5 ���Ӷ�������
	//��һ���������Ƕ����locationֵ
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); //��������OpenGL����ν����������ݣ�Ӧ�õ��������������)
	glEnableVertexAttribArray(0); //�Զ�������λ��ֵ��Ϊ���������ö������ԣ���������Ĭ���ǽ��õ�


	/*
	glfwWindowShouldClose����������ÿ��ѭ���Ŀ�ʼǰ���һ��GLFW�Ƿ�Ҫ���˳�������ǵĻ��ú�������trueȻ����Ⱦѭ��������ˣ�֮��Ϊ���ǾͿ��Թر�Ӧ�ó����ˡ�
	glfwPollEvents���������û�д���ʲô�¼�������������롢����ƶ��ȣ������´���״̬�������ö�Ӧ�Ļص�����������ͨ���ص������ֶ����ã���
	glfwSwapBuffers�����ύ����ɫ���壨����һ��������GLFW����ÿһ��������ɫֵ�Ĵ󻺳壩��������һ�����б��������ƣ����ҽ�����Ϊ�����ʾ����Ļ�ϡ�
	*/

	// ��Ⱦѭ��
	while (!glfwWindowShouldClose(window))
	{
		// ����
		processInput(window);

		// ��Ⱦָ��
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram); //������ɫ����������Ѽ�����ɫ���������ɫ���������Ƿ�����Ⱦ���õ�ʱ��ʹ��
		glBindVertexArray(VAO);     // ʹ��VAO�����ÿһ����Ⱦ��ʱ��ֱ��ʹ��VAO�洢�õ�����ָ��
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//6 �����VAO��VBO
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate(); //����Ⱦѭ��������������Ҫ��ȷ�ͷ�/ɾ��֮ǰ�ķ����������Դ
	return 0;
}

