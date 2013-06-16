#include "../headers/Controls.h"

Controls::Controls() {
	position = glm::vec3( 0, 0.6f, -2.5f ); 
	carPosition = glm::vec3(-2.66f, 0, 2.48);
	carDirection = glm::vec3(-(3.14/2),0,0);

	horizontalAngle = 3.14f;
	verticalAngle = 0.0f;
	initialFoV = 45.0f;
	rotationSpeed = 0.0f;
	speedPerTick = 1.0f;
	acceleration = 0.0f;
	steering = 0.0f;

	speed = 3.0f; 
	mouseSpeed = 0.005f;
}

void Controls::updateCamera(){
	static double lastTime = glfwGetTime();

	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);

	float FoV = initialFoV - 5;

	glm::vec3 temp = carDirection;

	temp.x = carDirection.y;
	temp.y = -carDirection.x;
	temp.z = carDirection.z;

	ProjectionMatrix = glm::perspective(FoV, 4.0f / 3.0f, 0.1f, 100.0f);
	ViewMatrix       = glm::lookAt(
								carPosition + position * glm::quat(temp), 	// Camera is here
								carPosition, 								// and looks here : at the same position, plus "direction"
								vec3(0, 1, 0)                  				// Head is up (set to 0,-1,0 to look upside-down)
						   );

	// For the next frame, the "last time" will be "now"
	lastTime = currentTime;
}

void Controls::updateCar(){
	static double lastTime = glfwGetTime();

	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);

	if(glfwGetKey( GLFW_KEY_LEFT ) == GLFW_PRESS) {
		steering += 0.03f;

		if(steering > 0.7f) 
			steering = 0.7f;
	} else if(glfwGetKey( GLFW_KEY_RIGHT ) == GLFW_PRESS) {
		steering -= 0.03f;

		if(steering < -0.7f) 
			steering = -0.7f;
	} else {
		if(steering > 0.01f) {
			steering -= 0.01f;
		}
		if(steering < 0.01f) {
			steering += 0.01f;
		}
	}
	
	if(acceleration > 0.0f)
		carDirection += vec3(glm::radians(steering), 0, 0);

	glm::mat4 RotationMatrix		= eulerAngleYXZ(carDirection.x, carDirection.y, carDirection.z);
	glm::vec4 forward				= RotationMatrix * glm::vec4(0,0,-1,0);
	glm::vec3 realforward(forward);

	if(glfwGetKey( GLFW_KEY_UP ) == GLFW_PRESS || glfwGetKey( GLFW_KEY_DOWN ) == GLFW_PRESS) {
		if (glfwGetKey( GLFW_KEY_UP ) == GLFW_PRESS) {
			rotationSpeed += speedPerTick;
			acceleration += speedPerTick * 0.015f;
			carPosition -= realforward * deltaTime * speed * acceleration;
		}
		if (glfwGetKey( GLFW_KEY_DOWN ) == GLFW_PRESS){
			if(acceleration > 0) {
				rotationSpeed -= speedPerTick;
				acceleration -= speedPerTick * 0.06f;
				carPosition -= realforward * deltaTime * speed * acceleration;
			}
		}
	} else {
		if(acceleration > 0.1f) {
			acceleration -= speedPerTick * 0.005f;
			carPosition -= realforward * deltaTime * speed * acceleration;
		}
		else if (acceleration < -0.1f){
			acceleration += speedPerTick * 0.005f;
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

glm::vec3 Controls::getPosition() {
	return position;
}

float Controls::getSteering() { 
	return steering;
}

float Controls::getRotationSpeed() {
	return rotationSpeed;
}

glm::vec3 Controls::getCarPosition() {
	return carPosition;
}

glm::vec3 Controls::getCarDirection() {
	return carDirection;
}

float Controls::getCarSpeed() {
	return acceleration;
}

void Controls::setCarSpeed(float accel) {
	acceleration = accel;
}