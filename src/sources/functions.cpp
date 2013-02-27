#include <../../src/headers/functions.h>
#include <../../src/headers/Texture.h>

enum {SKY_LEFT = 0,SKY_BACK,SKY_RIGHT,SKY_FRONT,SKY_TOP,SKY_BOTTOM, PLANE};      //constants for the skybox faces, so we don't have to remember so much number
unsigned int skybox[6]; //the ids for the textures

GLuint backvertexbuffer, frontvertexbuffer, rightvertexbuffer,leftvertexbuffer,bottomvertexbuffer,topvertexbuffer, planevertexbuffer;
GLuint uvbuffer;
GLuint SkyboxShaders, TextureID;

float boxsize = 75.0f;

static const GLfloat backquadBuffer[] = {
	boxsize/2, boxsize/2, boxsize/2,
	boxsize/2, -boxsize/2, boxsize/2,
	-boxsize/2, -boxsize/2, boxsize/2,
	-boxsize/2, boxsize/2, boxsize/2
};

static const GLfloat frontquadBuffer[] = {
	boxsize/2, boxsize/2, -boxsize/2,
	boxsize/2, -boxsize/2, -boxsize/2,
	-boxsize/2, -boxsize/2, -boxsize/2,
	-boxsize/2, boxsize/2, -boxsize/2
};

static const GLfloat leftquadBuffer[] = {
	-boxsize/2, boxsize/2, -boxsize/2,
	-boxsize/2, -boxsize/2, -boxsize/2,
	-boxsize/2, -boxsize/2, boxsize/2,
	-boxsize/2, boxsize/2, boxsize/2
};

static const GLfloat rightquadBuffer[] = {
	boxsize/2, boxsize/2, -boxsize/2,
	boxsize/2, -boxsize/2, -boxsize/2,
	boxsize/2, -boxsize/2, boxsize/2,
	boxsize/2, boxsize/2, boxsize/2
};

static const GLfloat bottomquadBuffer[] = {
	boxsize/2, -boxsize/2, -boxsize/2,
	boxsize/2, -boxsize/2, boxsize/2,
	-boxsize/2, -boxsize/2, boxsize/2,
	-boxsize/2, -boxsize/2, -boxsize/2
};

static const GLfloat topquadBuffer[] = {
	boxsize/2, boxsize/2, -boxsize/2,
	boxsize/2, boxsize/2, boxsize/2,
	-boxsize/2, boxsize/2, boxsize/2,
	-boxsize/2, boxsize/2, -boxsize/2
};


static const GLfloat planeBuffer[] = {
	-boxsize/8, 0, -boxsize/8,
	-boxsize/8, 0, boxsize/8,
	boxsize/8, 0, boxsize/8,
	boxsize/8, 0, -boxsize/8
};

static const GLfloat backTexBuffer[] = {
	// back quad
	0.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 1.0f,
    1.0f, 0.0f
};

void initEnvironment()
{
	Texture* tex = new Texture();
	skybox[SKY_LEFT]	= tex->loadBMP_customSky("C:/Users/Peter/Documents/GitHub/Racegame/src/media/left.bmp");
	tex = new Texture();
    skybox[SKY_BACK]	= tex->loadBMP_customSky("C:/Users/Peter/Documents/GitHub/Racegame/src/media/back.bmp");
	tex = new Texture();
    skybox[SKY_RIGHT]	= tex->loadBMP_customSky("C:/Users/Peter/Documents/GitHub/Racegame/src/media/right.bmp");
	tex = new Texture();
    skybox[SKY_FRONT]	= tex->loadBMP_customSky("C:/Users/Peter/Documents/GitHub/Racegame/src/media/front.bmp");
	tex = new Texture();
    skybox[SKY_TOP]		= tex->loadBMP_customSky("C:/Users/Peter/Documents/GitHub/Racegame/src/media/top.bmp");
	tex = new Texture();
    skybox[SKY_BOTTOM]	= tex->loadBMP_customSky("C:/Users/Peter/Documents/GitHub/Racegame/src/media/bottom.bmp");
	tex = new Texture();
	skybox[PLANE]		= tex->loadBMP_customSky("C:/Users/Peter/Documents/GitHub/Racegame/src/media/Racetrack.bmp");
		
	glGenBuffers(1, &backvertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, backvertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(backquadBuffer), backquadBuffer, GL_STATIC_DRAW);

	glGenBuffers(1, &frontvertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, frontvertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(frontquadBuffer), frontquadBuffer, GL_STATIC_DRAW);

	glGenBuffers(1, &rightvertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, rightvertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rightquadBuffer), rightquadBuffer, GL_STATIC_DRAW);

	glGenBuffers(1, &leftvertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, leftvertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(leftquadBuffer), leftquadBuffer, GL_STATIC_DRAW);
	
	glGenBuffers(1, &bottomvertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, bottomvertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(bottomquadBuffer), bottomquadBuffer, GL_STATIC_DRAW);
	
	glGenBuffers(1, &topvertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, topvertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(topquadBuffer), topquadBuffer, GL_STATIC_DRAW);

	glGenBuffers(1, &planevertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, planevertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeBuffer), planeBuffer, GL_STATIC_DRAW);

	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(backTexBuffer), backTexBuffer, GL_STATIC_DRAW);
}

void drawEnvironment()
{
	glDisable(GL_LIGHTING);
	glDisable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);

	drawQuad(SKY_BACK, backvertexbuffer, uvbuffer);
	drawQuad(SKY_FRONT, frontvertexbuffer, uvbuffer);
	drawQuad(SKY_RIGHT, rightvertexbuffer, uvbuffer);
	drawQuad(SKY_LEFT, leftvertexbuffer, uvbuffer);
	drawQuad(SKY_BOTTOM, bottomvertexbuffer, uvbuffer);
	drawQuad(SKY_TOP, topvertexbuffer, uvbuffer);
	drawQuad(PLANE, planevertexbuffer, uvbuffer);

	glEnable(GL_LIGHTING);
	glEnable(GL_CULL_FACE);
	glDisable(GL_TEXTURE_2D);
}

void drawQuad(int skyboxIndex, GLuint vertexbuffer, GLuint uvbuffer) {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, skybox[skyboxIndex]);

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(
		0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);
		
	// 2nd attribute buffer : UVs
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glVertexAttribPointer(
		1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		2,                                // size : U+V => 2
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	// Draw the triangle !
	glDrawArrays(GL_QUADS, 0, 4*2); 
}