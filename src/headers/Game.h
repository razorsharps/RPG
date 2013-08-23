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
#include "../headers/Door.h"
#include "../headers/Renderer.h"
#include "../headers/text2D.h"
#include "../headers/Sound.h"
#include "../headers/UpdateVisitor.h"
#include "../headers/Collision.h"
#include "../headers/Key.h"
#include "../headers/Inventory.h"
#include "../headers/Octree.h"
#include "../headers/Rocket.h"

class Game {
	enum {NORMAL = 0};
	GLuint shaders[3]; 

	enum {MATRIXID = 0, VIEWMATRIXID, MODELMATRIXID};
	GLuint handles[4]; 

	private:
		int checkIndex;
		GLuint TextureIDs;

		Astroid* selected;
		GameObject *halo;
		Rocket* rocket;

		double startTime;

		bool carMoved;
		bool playSound;
		bool win;

		Environment *environment;
		Controls* control;
		Collision* collision;
		UpdateVisitor updateVisitor;
		Octree* octree;

		std::vector<Rocket*> rockets;

		float pie;
	public:
		
		Game();
		virtual ~Game();

		void initGLFW();
		void initGLEW();
		
		void setGlParameters();
		void buildGameObjects();

		void foundKey(Key* aKey);

		void build();
		void run();
		void stop();

		GameObject * GetGameObjectFromPosition(glm::vec3 position);
		void removeObject(GameObject* gameObject);

		Renderer* renderer;

		GameObject* collisionObject;
		std::vector<GameObject*> collisionObjects;

		Mesh * explosionObject;
		Texture * collisionTexture;

		void addCollisionObject(glm::vec3 pos);
};