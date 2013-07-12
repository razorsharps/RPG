#include "../headers/Controls.h"
#include "../headers/GameObject.h"
#include <iostream>

Controls::Controls() {
	position = glm::vec3( 0,0.05f, -0.15f ); 
	cameraPosition = glm::vec3(0.0f);
	carPosition = glm::vec3(0.0f, 0.0f, 0.0f );
	carDirection = glm::vec3(0,0,0);

	horizontalAngle = 0.0f;
	verticalAngle = 0.0f;
	initialFoV = 45.0f;
	rotationSpeed = 0.0f;
	speedPerTick = 1.0f;
	acceleration = 0.0f;
	horizontalSteering = 0.0f;
	verticalSteering = 0.0f;

	speed = 3.0f; 
	mouseSpeed = 0.005f;
}

void Controls::updateCamera(){
	static double lastTime = glfwGetTime();

	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);

	float FoV = initialFoV - 5;
	//(*it)->getParent()->getPosition() + (*it)->getOrientationQuat() * (*it)->getPosition();
	glm::vec3 lookAt = carPosition;

	glm::vec3 temp;

	temp.x = spaceShip->orientation.y;
	temp.y = spaceShip->orientation.x;
	temp.z = spaceShip->orientation.z;

	glm::mat4 RotationMatrix		= eulerAngleYXZ(carDirection.x, carDirection.y, carDirection.z);
	glm::vec4 up = RotationMatrix * glm::vec4(0,1,0,0);
	glm::vec3 up3(up);

	glm::vec3 cameraOptimalposition = carPosition + glm::quat(temp) * position;
	ProjectionMatrix = glm::perspective(FoV, 4.0f / 3.0f, 0.1f, 100.0f);

	glm::mat4 direction = glm::inverse(glm::lookAt(cameraOptimalposition, cameraPosition, glm::vec3(0.0f,1.0f,0.0f)));
	glm::vec4 forward	= direction * glm::vec4(0,0,-1,0);
	glm::vec3 realforward(forward);

	cameraPosition = realforward * 0.1f + carPosition + glm::quat(temp) * position;


	ViewMatrix       = glm::lookAt(
								cameraPosition, 	// Camera is here
								lookAt, 									// and looks here : at the same position, plus "direction"
								up3                				// Head is up (set to 0,-1,0 to look upside-down)
						   );

	// For the next frame, the "last time" will be "now"
	lastTime = currentTime;
}

void Controls::update(){
	static double lastTime = glfwGetTime();

	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);

	if(glfwGetKey('A') == GLFW_PRESS) {
		horizontalSteering += 0.02f;

		if(horizontalSteering > 0.7f) 
			horizontalSteering = 0.7f;
	} else if(glfwGetKey('D') == GLFW_PRESS) {
		horizontalSteering -= 0.02f;

		if(horizontalSteering < -0.7f) 
			horizontalSteering = -0.7f;
	} else {
		if(horizontalSteering < 0.1f && horizontalSteering > -0.1f)
			horizontalSteering = 0;
		else if(horizontalSteering > 0) {
			horizontalSteering -= 0.02f;
		} else {
			horizontalSteering += 0.02f;
		}
	}

	if(glfwGetKey('Q') == GLFW_PRESS) {
		verticalSteering += 0.02f;

		if(verticalSteering > 0.7f) 
			verticalSteering = 0.7f;
	} else if(glfwGetKey('E') == GLFW_PRESS) {
		verticalSteering -= 0.02f;

		if(verticalSteering < -0.7f) 
			verticalSteering = -0.7f;
	} else {
		if(verticalSteering < 0.1f && verticalSteering > -0.1f)
			verticalSteering = 0;
		else if(verticalSteering > 0) {
			verticalSteering -= 0.02f;
		} else {
			verticalSteering += 0.02f;
		}
	}
	
	carDirection += vec3(glm::radians(horizontalSteering), glm::radians(verticalSteering), 0);

	glm::mat4 RotationMatrix		= eulerAngleYXZ(carDirection.x, carDirection.y, carDirection.z);
	glm::vec4 forward				= RotationMatrix * glm::vec4(0,0,-1,0);
	glm::vec3 realforward(forward);

	if(glfwGetKey('W') == GLFW_PRESS || glfwGetKey('S') == GLFW_PRESS) {
		if (glfwGetKey('W') == GLFW_PRESS) {
			rotationSpeed += speedPerTick;
			acceleration += speedPerTick * 0.06f;

			if(acceleration > 1)
				acceleration = 1;

			carPosition -= realforward * deltaTime * speed * acceleration;
		}
		if (glfwGetKey('S') == GLFW_PRESS){
			rotationSpeed -= speedPerTick;
			acceleration -= speedPerTick * 0.06f;

			if(acceleration < -1)
				acceleration = -1;

			carPosition -= realforward * deltaTime * speed * acceleration;
		}
	} else {
		if(acceleration > 0.1f) {
			acceleration -= speedPerTick * 0.02f;
			carPosition -= realforward * deltaTime * speed * acceleration;
		}
		else if (acceleration < -0.1f){
			acceleration += speedPerTick * 0.02f;
			carPosition -= realforward * deltaTime * speed * acceleration;
		} else
			acceleration = 0;
	}

	// For the next frame, the "last time" will be "now"
	lastTime = currentTime;
}


glm::mat4 Controls::getViewMatrix(){
	return ViewMatrix;
}

glm::mat4 Controls::getProjectionMatrix(){
	return ProjectionMatrix;
}

glm::vec3 Controls::getCameraPosition() {
	return position;
}

float Controls::getSteering() { 
	return horizontalSteering;
}

float Controls::getRotationSpeed() {
	return rotationSpeed;
}

glm::vec3 Controls::getPosition() {
	return carPosition;
}

glm::vec3 Controls::getDirection() {
	return carDirection;
}

float Controls::getSpeed() {
	return acceleration;
}

void Controls::setSpeed(float accel) {
	acceleration = accel;
}

void Controls::accept(Visitor &v) {
	v.visit(this);
}