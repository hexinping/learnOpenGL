#include "OpenglUtils.h"
#include "std_image.h"

static OpenglUtils *glUtils = nullptr;

OpenglUtils * OpenglUtils::getInstance()
{
	if (!glUtils)
	{
		glUtils = new (std::nothrow) OpenglUtils();
	}
	return glUtils;
}

char* OpenglUtils::readTheFile(string strSource) {
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

void OpenglUtils::bindVBOAndVAO(unsigned int *VBO, unsigned int *VAO, float vertices[], int verticesLenght, bool useEBO, unsigned int *EBO, unsigned int indices[], int indicesLenght)
{
	// 1 �������ݴ���
	//float vertices[] = {
	//	-0.5f, -0.5f, 0.0f,
	//	0.5f, -0.5f, 0.0f,
	//	0.0f, 0.5f, 0.0f
	//};

	//float vertices[] = {
	//	0.5f, 0.5f, 0.0f,   // ���Ͻ�
	//	0.5f, -0.5f, 0.0f,  // ���½�
	//	-0.5f, -0.5f, 0.0f, // ���½�
	//	-0.5f, 0.5f, 0.0f   // ���Ͻ�
	//};

	//unsigned int indices[] = { // ע��������0��ʼ! 
	//	0, 1, 3, // ��һ��������
	//	1, 2, 3  // �ڶ���������
	//};

	glGenVertexArrays(1, VAO); //����һ��vao��ֻҪ�Ȼ�һ�飬vao���¼���Ƶ����ԣ�Ȼ������ѭ���ﲻ���л�vao�ͺ�

	glBindVertexArray(*VAO); //�����Ȱ�VAO

	glGenBuffers(1, VBO); //����һ��VBO����:�൱�����Դ��Ͽ���һ��洢�ռ�

	glBindBuffer(GL_ARRAY_BUFFER, *VBO);//glBindBuffer�������´����Ļ���󶨵�GL_ARRAY_BUFFERĿ����

	glBufferData(GL_ARRAY_BUFFER, verticesLenght, vertices, GL_STATIC_DRAW);//�Ѷ���Ķ������ݸ��Ƶ�������ڴ��У�VBO��


	//�������ǵ��������鵽һ�����������У���OpenGLʹ��
	if (useEBO)
	{
		glGenBuffers(1, EBO); //����һ������������
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *EBO); //glBindBuffer�������´����Ļ���󶨵�GL_ELEMENT_ARRAY_BUFFERĿ����
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesLenght, indices, GL_STATIC_DRAW); //�Ѷ���Ķ������ݸ��Ƶ�������ڴ��У�EBO��
	}

}

void  OpenglUtils::createShaderWithSource(int shaderType, unsigned int *pShader, const char *shaderSource)
{
	unsigned int shader;
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

void  OpenglUtils::createShaderWithFile(int shaderType, unsigned int *pShader, string file)
{
	char * shaderSource = readTheFile(file);
	createShaderWithSource(shaderType, pShader, shaderSource);
}

int OpenglUtils::linkShader(unsigned int *shaderProgram, unsigned int vertexShader, unsigned int fragmentShader)
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

GLint OpenglUtils::getUniformLocation(unsigned int program, const char *name)
{
	return glGetUniformLocation(program, name);
}

GLint OpenglUtils::getAttribLocation(unsigned int program, const char *name)
{
	return glGetAttribLocation(program, name);
}

void OpenglUtils::setBool(unsigned int program, const char *name, bool value) const
{
	glUniform1i(glGetUniformLocation(program, name), (int)value);
}
void OpenglUtils::setInt(unsigned int program, const char *name, int value) const
{
	glUniform1i(glGetUniformLocation(program, name), value);
}
void OpenglUtils::setFloat(unsigned int program, const char *name, float value) const
{
	glUniform1f(glGetUniformLocation(program, name), value);
}

void OpenglUtils::genTexImage2D(const char *file, int type, int level, int textureUnit, int around, int filter)
{
	unsigned int texture;
	glGenTextures(1, &texture);
	glActiveTexture(textureUnit); // �ڰ�����֮ǰ�ȼ�������Ԫ

	glBindTexture(GL_TEXTURE_2D, texture);
	// Ϊ��ǰ�󶨵�����������û��ơ����˷�ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, around);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, around);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);


	//��������
	int width, height, nrChannels;
	unsigned char *data = stbi_load(file, &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, level, type, width, height, 0, type, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_set_flip_vertically_on_load(true);
	stbi_image_free(data);

}

void OpenglUtils::useProgram(unsigned int program)
{
	glUseProgram(program);
}
