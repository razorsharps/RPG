#ifndef GAME_H
#define GAME_H

#include <GL/glew.h>
#include <GL/glfw.h>
#include <glm/glm.hpp>
#include <vector>
#include <algorithm>

class Game {
	private:
		
	public:
		Game();
		virtual ~Game();

		void initGLFW();
		void initGLEW();
		void setGlParameters();

		void build();
		void run();
		void stop();
};

#endif