#include "../headers/Renderer.h"

Renderer::Renderer() {
}

Renderer::~Renderer() {
	
}

void Renderer::renderObjects(glm::mat4 ProjectionMatrix, glm::mat4 ViewMatrix, GLuint matrixid, GLuint modelmatrixid, GLuint viewmatrixid) {
	std::vector<GameObject*>::iterator it;

	for(it = gameObjects.begin(); it != gameObjects.end(); ++it)
	{
		glm::vec3 rotation;
		glm::vec3 position;
		glm::vec3 scaling;

		/*if((*it)->hasParent()) {
			rotation	= (*it)->getParent()->getOrientation() + (*it)->getOrientation();
			position	= (*it)->getParent()->getPosition() + (*it)->getOrientationQuat() * (*it)->getPosition();
			scaling		= (*it)->getScaling();

			if((*it)->isSteering())
				rotation	+= (*it)->getSteering();
		} 
		else {*/
			rotation	= (*it)->getOrientation();
			position	= (*it)->getPosition();
			scaling		= (*it)->getScaling();

		//}
		// Build the model matrix
		glm::mat4 RotationMatrix;
		RotationMatrix = glm::eulerAngleYXZ(rotation.x, rotation.y, rotation.z);

		glm::mat4 TranslationMatrix		= glm::translate(glm::mat4(), position); 
		glm::mat4 ScalingMatrix			= glm::scale(glm::mat4(), scaling);
		glm::mat4 ModelMatrix			= TranslationMatrix * RotationMatrix * ScalingMatrix;

		glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

		// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform
		glUniformMatrix4fv(matrixid, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(modelmatrixid, 1, GL_FALSE, &ModelMatrix[0][0]);
		glUniformMatrix4fv(viewmatrixid, 1, GL_FALSE, &ViewMatrix[0][0]);	
		(*it)->accept(renderVisitor);
	}
}

void Renderer::addObjects(GameObject* gameObject) {
	gameObjects.push_back(gameObject);
}