#ifndef GAME_H
#define GAME_H

#include <GL/glew.h>
#include <GL/glfw.h>
#include <glm/glm.hpp>
#include <vector>
#include <algorithm>

// An array of 3 vectors which represents 3 vertices
static const GLfloat vertexPositions[] = {
   -1.0f, -1.0f, 0.0f,
   1.0f, -1.0f, 0.0f,
   0.0f,  1.0f, 0.0f,
};

class Game {
	private:
		
	public:
		Game();
		virtual ~Game();
};

#endif