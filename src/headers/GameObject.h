#pragma once

// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>

#include <GL/glew.h>
#include <GL/glfw.h>
#include <glm/glm.hpp>
#include <vector>
#include <algorithm>

#include <glm/gtc/quaternion.hpp>

#include "../headers/objloader.h"
#include "../headers/Mesh.h"

class Mesh;

class GameObject {
	private:
		std::string name;
		
		GameObject* _parent;

		float rotationSpeed;

		glm::vec3 position;
		glm::vec3 orientation;
		glm::vec3 scaling;
		glm::vec3 steering;

		bool steerable;
		bool tireBehaviour;

		Texture * colorMap;

		GLuint vertexPosition_modelspaceID;
		GLuint vertexUVID;
		GLuint vertexNormal_modelspaceID;
	public:
		GameObject(std::string aName, glm::vec3 aPosition, float scale, bool steer = false, glm::vec3 orientation = glm::vec3(0,0,0));
		virtual ~GameObject();

		void translateObject(glm::vec3 translation);
		void rotateObject(glm::vec3 axis );
		void scaleObject(glm::vec3 scale);

		void render();
		void init(GLuint shader);

		const std::string getName();

		bool hasParent();
		bool hasTireBehaviour();
		bool isSteering();

		glm::vec3 getPosition();
		glm::vec3 getOrientation();
		glm::vec3 GameObject::getSteering();
		glm::vec3 getScaling();
		glm::quat getOrientationQuat();
		float getRotationSpeed();
		GameObject* getParent();

		void setTexture(Texture& tex);
		void setMesh(Mesh* mes);
		void setParent(GameObject& gameObject);
		void setTireBehaviour();
		void setRotationSpeed(float speed);
		void setSteering(glm::vec3 steer);

		Mesh * mesh;
};