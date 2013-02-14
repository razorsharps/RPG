#ifndef GAME_H
#define GAME_H

#include <GL/glew.h>
#include <GL/glfw.h>
#include <glm/glm.hpp>
#include <vector>
#include <algorithm>

const float vertexPositions[] = {
	0.75f, 0.75f, 0.0f, 1.0f,
	0.75f, -0.75f, 0.0f, 1.0f,
	-0.75f, -0.75f, 0.0f, 1.0f,
};

const std::string strVertexShader(
	"#version 330\n"
	"layout(location = 0) in vec4 position;\n"
	"void main()\n"
	"{\n"
	"   gl_Position = position;\n"
	"}\n"
);

const std::string strFragmentShader(
	"#version 330\n"
	"out vec4 outputColor;\n"
	"void main()\n"
	"{\n"
	"   outputColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"
	"}\n"
);

class Game {
	private:
		GLuint* theProgram;
		GLuint* positionBufferObject;
		GLuint* vao;
	public:
		void display();
		GLuint CreateShader(GLenum,  const std::string &);
		GLuint CreateProgram(const std::vector<GLuint> &);
		void InitializeVertexBuffer();
		void InitializeProgram();
};

#endif