#pragma once

// Include GLFW
#include <GL/glfw.h>

class Time {

	public:
		static Time& getInstance()
		{
			static Time    instance;	// Guaranteed to be destroyed.
										// Instantiated on first use.
			return instance;
		}

		static double getTime() {
			return glfwGetTime();
		}

	private:
		Time() {};

		Time(Time const&);             
		void operator=(Time const&); 
};