/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#include <iostream>

#include "Texture.h"


Texture2D::Texture2D()
    : Width(0), Height(0), Internal_Format(GL_RGB), Image_Format(GL_RGB), Wrap_S(GL_REPEAT), Wrap_T(GL_REPEAT), Filter_Min(GL_LINEAR), Filter_Max(GL_LINEAR)
{
    glGenTextures(1, &this->ID);
}

void Texture2D::Generate(GLuint width, GLuint height, unsigned char* data)
{
    Width = width;
    Height = height;

	glActiveTexture(TextureUnit); // 在绑定纹理之前先激活纹理单元
	glBindTexture(GL_TEXTURE_2D, ID);

	/*
	0 ==》纹理级别，默认使用0级
	纹理格式 format
	*/
	glTexImage2D(GL_TEXTURE_2D, 0, Internal_Format, width, height, 0, Image_Format, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	// 纹理环绕方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, Internal_Format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT); // for this tutorial: use GL_CLAMP_TO_EDGE to prevent semi-transparent borders. Due to interpolation it takes texels from next repeat 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, Internal_Format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);

	//纹理过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::Bind() const
{
    glBindTexture(GL_TEXTURE_2D, this->ID);
}