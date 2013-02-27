#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <GL/glew.h>

#include <GL/glfw.h>

#include "../headers/Texture.h"

Texture::Texture() {

}

Texture::~Texture() {

}

GLuint Texture::loadBMP_custom(const char * imagepath){
	printf("Reading image %s\n", imagepath);

	unsigned char header[54];
	unsigned int dataPos;
	unsigned int imageSize;
	unsigned int width, height;
	unsigned char * data;

	FILE * file = fopen(imagepath,"rb");
	if (!file)							    {printf("%s could not be opened. Are you in the right directory ? Don't forget to read the FAQ !\n", imagepath); return 0;}


	if ( fread(header, 1, 54, file)!=54 ){ 
		printf("Not a correct BMP file\n");
	}
	if ( header[0]!='B' || header[1]!='M' ){
		printf("Not a correct BMP file\n");
	}
	if ( *(int*)&(header[0x1E])!=0  )         {printf("Not a correct BMP file\n");    return 0;}
	if ( *(int*)&(header[0x1C])!=24 )         {printf("Not a correct BMP file\n");    return 0;}

	dataPos    = *(int*)&(header[0x0A]);
	imageSize  = *(int*)&(header[0x22]);
	width      = *(int*)&(header[0x12]);
	height     = *(int*)&(header[0x16]);

	if (imageSize==0)    imageSize=width*height*3; 
	if (dataPos==0)      dataPos=54; 

	data = new unsigned char [imageSize];

	fread(data,1,imageSize,file);

	fclose (file);

	GLuint textureID;
	glGenTextures(1, &textureID);
	
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

	delete [] data;

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
	glGenerateMipmap(GL_TEXTURE_2D);

	_texture = textureID;
	return _texture;
}

GLuint Texture::loadBMP_customSky(const char * imagepath){

	printf("Reading image %s\n", imagepath);

	unsigned char header[54];
	unsigned int dataPos;
	unsigned int imageSize;
	unsigned int width, height;
	unsigned char * data;

	FILE * file = fopen(imagepath,"rb");
	if (!file)							    {printf("%s could not be opened. Are you in the right directory ? Don't forget to read the FAQ !\n", imagepath); return 0;}

	if ( fread(header, 1, 54, file)!=54 ){ 
		printf("Not a correct BMP file\n");
	}
	if ( header[0]!='B' || header[1]!='M' ){
		printf("Not a correct BMP file\n");
	}
	if ( *(int*)&(header[0x1E])!=0  )         {printf("Not a correct BMP file\n");    return 0;}
	if ( *(int*)&(header[0x1C])!=24 )         {printf("Not a correct BMP file\n");    return 0;}

	dataPos    = *(int*)&(header[0x0A]);
	imageSize  = *(int*)&(header[0x22]);
	width      = *(int*)&(header[0x12]);
	height     = *(int*)&(header[0x16]);

	if (imageSize==0)    imageSize=width*height*3; 
	if (dataPos==0)      dataPos=54; 

	data = new unsigned char [imageSize];

	fread(data,1,imageSize,file);

	fclose (file);

	GLuint textureID;
	glGenTextures(1, &textureID);

	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

	delete [] data;

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
	glGenerateMipmap(GL_TEXTURE_2D);

	_texture = textureID;
	return _texture;
}

GLuint Texture::getTexture() {
	return _texture;
}