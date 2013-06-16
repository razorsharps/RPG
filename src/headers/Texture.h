#pragma once

class Texture {
	private:
		GLuint _texture;

	public:
		Texture();
		virtual ~Texture();

		GLuint loadBMP_custom(const char * imagepath);
		GLuint loadBMP_customSky(const char * imagepath);
		GLuint loadTGA_glfw(const char * imagepath);
		GLuint getTexture();
};
