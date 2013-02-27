// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
 
// Include GLEW
#include <GL/glew.h>
 
// Include GLFW
#include <GL/glfw.h>

#include "../headers/Game.h"
#include "../headers/Shader.h"
#include "../headers/Texture.h"
#include "../headers/objloader.h"
#include "../headers/vboindexer.h"
#include "../headers/functions.h"
#include "../headers/controls.h"
#include "../headers/Mesh.h"

class GameObject {
	private:
		std::string name;
		GameObject* parent;
		glm::mat4 transform;

		Mesh * mesh;
		Texture * colorMap;

	public:
		GameObject(std::string aName = NULL, glm::vec3 aPosition = glm::vec3( 0.0f, 0.0f, 0.0f ));
		virtual ~GameObject();

		void translate(glm::vec3 translation);
		void rotate( float angle, glm::vec3 axis );

		glm::vec3 getLocation();

		const std::string getName();

		void setTexture(Texture* tex);
		void setMesh(Mesh* mes);
};
