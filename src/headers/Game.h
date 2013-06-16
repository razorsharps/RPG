#pragma once

#include <GL/glew.h>
#include <GL/glfw.h>
#include <glm/glm.hpp>
#include <vector>
#include <algorithm>

#include "../headers/Shader.h"
#include "../headers/Texture.h"
#include "../headers/objloader.h"
#include "../headers/Environment.h"
#include "../headers/controls.h"
#include "../headers/GameObject.h"
#include "../headers/Renderer.h"
#include "../headers/text2D.h"
#include "../headers/Sound.h"

class Game {
	enum {NORMAL = 0, SKYBOX};
	GLuint shaders[3]; 

	enum {MATRIXID = 0, VIEWMATRIXID, MODELMATRIXID, SKYBOXMATRIXID};
	GLuint handles[4]; 

	enum {CHECK1 = 0, CHECK2, CHECK3, CHECK4, CHECK5, CHECK6};
	glm::vec3 checkpoints[6]; 

	private:
		int checkIndex;

		GLuint TextureIDs;
		GLuint LightID;

		Renderer* renderer;

		GameObject *car, *tire1, *tire2, *tire3, *tire4;
		GameObject *box1, *box2;

		double startTime;

		bool carMoved;
		bool playSound;
		float bestLap;

		Environment *environment;
		Controls* control;

		float pie;
	public:
		Game();
		virtual ~Game();

		void initGLFW();
		void initGLEW();
		void initCheckPoints();
		
		void setGlParameters();
		void buildGameObjects();

		void checkLapTime();

		void build();
		void run();
		void stop();
};