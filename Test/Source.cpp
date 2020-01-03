#include <windows.h>		// Header GL_EXPFile For Windows
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <glm/gtc/type_ptr.hpp>

#include "texture.h"
#include "gltexture.h"
#include "Model_3DS.h"
#include "Camera.h"
#include <math.h>

void mouse_callback(GLfloat xpos, GLfloat ypos);
void CreatModel(Model_3DS *Model, float x, float y, float z, float scale);
void Land(float x1, float z1, float x2, float z2, int Texture, GLfloat n); //
void Wall(float size, float sx, float sy, float sz, float alpha, int texture, GLfloat n);
void Road(float x, float z, int Texture);
void Road2(float x1, float z1, float x2, float z2, int Texture);
void build(GLfloat size, GLfloat x, GLfloat z, GLfloat width, GLfloat length, GLfloat Depth, int texture);
void Skybox(GLfloat size);
void scroll_callback(GLfloat xoffset, GLfloat yoffset);
void inParkShape();

void ModelLoading();
void TextureLoading();

void SecondLandDraw();
void Park();
void Mal();
void Buildings();
void processInput();

void Park();
HDC			hDC = NULL;		// Private GDI Device Context
HGLRC		hRC = NULL;		// Permanent Rendering Context
HWND		hWnd = NULL;		// Holds Our Window Handle
HINSTANCE	hInstance;		// Holds The Instance Of The Application

bool	keys[256];			// Array Used For The Keyboard Routine
bool	active = TRUE;		// Window Active Flag Set To TRUE By Default
bool	fullscreen = FALSE;	// Fullscreen Flag Set To Fullscreen Mode By Default

float lastX = 800 / 2.0f;
float lastY = 600 / 2.0f;
bool firstMouse = true;

// box and land size
GLfloat Size = 50;
GLfloat LandSize = -Size / 2 + 10;
Camera camera(glm::vec3(2.0f, LandSize + 1.1, 2.0f), LandSize + 1.9);
Camera camera2(glm::vec3(-33.0f, LandSize + 1.10, -33.0f), LandSize + 20);

// timing
float deltaTime = 0.1f;	

LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// Declaration For WndProc


GLvoid ReSizeGLScene(GLsizei width, GLsizei height)		// Resize And Initialize The GL Window
{
	if (height == 0)										// Prevent A Divide By Zero By
	{
		height = 1;										// Making Height Equal One
	}

	glViewport(0, 0, width, height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

										 				// Calculate The Aspect Ratio Of The Window
	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 2, 100000);
	
	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
}

//For SkyBox
int  top;
int  front;
int  back;
int  down;
int  right;
int  left;
int reflectedTex;

int  tranwall;
int  yellowwall;
int  whitewall;
int  blackwall;
int  redwall;

int  Sidewalk;
int  downLand;
int  ParkLand;

GLint Building_1[6];
GLint SecondLand;
GLint mallRoad;

Model_3DS * TreeModel;
Model_3DS *Hydrant;
Model_3DS *Street_Model;
Model_3DS *fence;
Model_3DS *Wood_Bench;
Model_3DS *SLIDING_ZOOLA;
Model_3DS *city_stall;
Model_3DS *Tree_1;
Model_3DS *Tree_2;
Model_3DS *Tree_3;

Model_3DS *grass_block;
Model_3DS *House; 
Model_3DS *House_1;

// moonlight
GLfloat moonLightAmbient[]= { 0.3f, 0.3f, 0.3f, 1.0f }; 
GLfloat moonLightDiffuse[]= { 0.3f, 0.3f, 0.3f, 1.0f }; 
GLfloat moonLightPosition[]= { 0.0f, LandSize + 20.0f, 2.0f, 1.0f }; 
GLboolean darkMode = false;

// sunlight
float downToTop;
GLfloat sunLightAmbient[]= { 1.0f, 1.0f, 1.0f, 1.0f }; 
GLfloat sunLightDiffuse[]= { 1.0f, 1.0f, 1.0f, 1.0f }; 
GLfloat sunLightPosition[]= { 1.0f, LandSize + 55.0f, 0.0f, 8.0f }; 

// for fog 
GLfloat fogColor[4]= {0.5f, 0.5f, 0.5f, 1.0f}; // Fog Color
GLboolean fog = false;

void TextureLoading() { //Skybox
	front = LoadTexture((char*)"Photos\\SkyBoxLand\\pz.png");
	back = LoadTexture((char*)"Photos\\SkyBoxLand\\nz.png");
	top = LoadTexture((char*)"Photos\\SkyBoxLand\\py.png");
	down = LoadTexture((char*)"Photos\\SkyBoxLand\\ny.png");
	right = LoadTexture((char*)"Photos\\SkyBoxLand\\nx.png");
	left = LoadTexture((char*)"Photos\\SkyBoxLand\\px.png");
	reflectedTex = LoadTexture((char*)"Photos\\SkyBoxLand\\reflected.png");

	Building_1[0] = LoadTexture((char*)"Photos/Build/building_type_1.jpg");
	Building_1[1] = LoadTexture((char*)"Photos/Build/building_type_2.jpg");
	Building_1[3] = LoadTexture((char*)"Photos/Build/building_type_4.jpg");
	Building_1[4] = LoadTexture((char*)"Photos/Build/building_type_5.jpg");
	Building_1[5] = LoadTexture((char*)"Photos/Build/building_type_6.jpg");

	tranwall = LoadTexture((char*)"Photos\\Build\\mall_outer_wall.jpg");
	whitewall = LoadTexture((char*)"Photos\\Build\\mall_floor.jpg");
	blackwall = LoadTexture((char*)"Photos\\Build\\brownwall.jpg");
	Sidewalk = LoadTexture((char*)"Photos\\Sidewalk.jpg");

	ParkLand = LoadTexture((char*)"Photos\\ParkLand.jpg");
	downLand = LoadTexture((char*)"Photos\\downLand.jpg");
	SecondLand = LoadTexture((char*)"Photos\\ParkLand1.jpg");
	mallRoad = LoadTexture((char*)"Photos/mall_road.jpg");
	

}

void ModelLoading() {


	TreeModel = new Model_3DS();
	TreeModel->Load("Models\\Park\\M_TREE5.3DS");
	
	GLTexture BARK, Leaf;
	Leaf.LoadBMP("Photos\\Leaf.bmp");
	BARK.LoadBMP("Photos\\Bark.bmp");

	TreeModel->Materials[0].tex = BARK;
	TreeModel->Materials[1].tex = Leaf;


	/*Hydrant = new Model_3DS();
	Hydrant->Load("Hydrant.3ds");

	GLTexture HydrantColor;
	HydrantColor.LoadBMP("red.bmp");

	Hydrant->Materials[0].tex = HydrantColor;
	*/
	Street_Model = new Model_3DS();
	Street_Model->Load("Models\\road.3ds");

	GLTexture Street;
	Street.LoadBMP("Photos/roadtexture.bmp");

	Street_Model->Materials[0].tex = Street;
	
	
	fence = new Model_3DS();
	fence->Load("Models\\Park\\fence.3ds");

	GLTexture fence_1, fence_2, fence_3, fence_4;
	//fence_1.LoadBMP("fence_1.bmp");
	fence_2.LoadBMP("Photos/fence_2.bmp");
	//fence_3.LoadBMP("fence_3.bmp");
    //fence_4.LoadBMP("fence_4.bmp");
	
	fence->Materials[0].tex = fence_2;
    fence->Materials[1].tex = fence_2;
	fence->Materials[2].tex = fence_2;
	fence->Materials[3].tex = fence_2;
	
	Wood_Bench = new Model_3DS();
	Wood_Bench->Load("Models\\Park\\Wood_Bench.3ds");

	GLTexture Wood;
	Wood.LoadBMP("Photos\\Wood.bmp");
	Wood_Bench->Materials[0].tex = Wood;


	SLIDING_ZOOLA = new Model_3DS();
	SLIDING_ZOOLA->Load("SLIDING_ZOOLA.3ds");

	GLTexture SLIDING_1, SLIDING_2;
	SLIDING_1.LoadBMP("Photos\\SLIDING ZOOLA.bmp");
	SLIDING_2.LoadBMP("Photos\\SLIDING ZOOLA 2.bmp");

	SLIDING_ZOOLA->Materials[0].tex = SLIDING_1;
	SLIDING_ZOOLA->Materials[1].tex = SLIDING_1;
	SLIDING_ZOOLA->Materials[2].tex = SLIDING_2;
	SLIDING_ZOOLA->Materials[3].tex = SLIDING_2;

	city_stall = new Model_3DS();
	city_stall->Load("city_stall.3ds");

	GLTexture stall_s, stall_n, stall_d;
	stall_s.LoadBMP("Photos\\city_stall_s.bmp");
	stall_d.LoadBMP("Photos\\city_stall_d.bmp");
	stall_n.LoadBMP("Photos\\city_stall_n.bmp");

	city_stall->Materials[0].tex = stall_s;
	city_stall->Materials[1].tex = stall_d;
	city_stall->Materials[2].tex = stall_n;

	grass_block = new Model_3DS();
	grass_block->Load("Models\\Park\\grass-block.3ds");

	GLTexture grass;
	grass.LoadBMP("Photos\\grass.bmp");

	grass_block->Materials[3].tex = grass;

	House = new Model_3DS();
	House->Load("House.3ds");

	GLTexture Roof,Wall_1,Wall_2;
	Roof.LoadBMP("roof.bmp");
	Wall_1.LoadBMP("wall.bmp");
	Wall_2.LoadBMP("wall1.bmp");

	House->Materials[0].tex = BARK;
	House->Materials[1].tex = Wall_1;
	House->Materials[2].tex = Wall_1;
	House->Materials[3].tex = BARK;
	House->Materials[4].tex = Wall_1;
	House->Materials[5].tex = Wall_1;
	House->Materials[6].tex = Wall_1;
	House->Materials[7].tex = Wall_2;
	House->Materials[8].tex = BARK;
 
}



int InitGL(GLvoid)										// All Setup For OpenGL Goes Here
{
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(1, 1, 1.0f, 0.5f);				        // Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glEnable(GL_TEXTURE_2D);
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
														//our Enable


	// sunglight
	glLightfv(GL_LIGHT0, GL_AMBIENT, sunLightAmbient); 
	glLightfv(GL_LIGHT0, GL_DIFFUSE, sunLightDiffuse); 
	glLightfv(GL_LIGHT0, GL_POSITION, sunLightPosition); 
	glEnable(GL_LIGHT0); 

	// moonlight
	glLightfv(GL_LIGHT1, GL_AMBIENT, moonLightAmbient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, moonLightDiffuse);	
	glLightfv(GL_LIGHT1, GL_POSITION, moonLightPosition);
	glDisable(GL_LIGHT1);  // disable moonlight by default

	glEnable(GL_LIGHTING);

	// fog
	glFogi(GL_FOG_MODE, GL_EXP);
	glFogfv(GL_FOG_COLOR, fogColor);

	glFogf(GL_FOG_DENSITY, 0.05f); 
	glHint(GL_FOG_HINT, GL_NICEST);

	glFogf(GL_FOG_START, 1.0f); 
	glFogf(GL_FOG_END, 5.0f); 


	TextureLoading();
	ModelLoading();
	return TRUE;									    	// Initialization Went OK
}


//For Camera
void processInput()
{

	// camera
	if (keys['A'])
	{
        camera.ProcessKeyboard(LEFT, deltaTime);
	}
	if (keys['D'])
	{
        camera.ProcessKeyboard(RIGHT, deltaTime);
	}
	if (keys['S'])
	{
        camera.ProcessKeyboard(BACKWARD, deltaTime);
	}
	if (keys['W'])
	{
        camera.ProcessKeyboard(FORWARD, deltaTime);
	}

	// lighting 
	if (keys['L']) 
	{
		if (!darkMode) {
			glEnable(GL_LIGHT1);
			glDisable(GL_LIGHT0);
			darkMode = true;
		}

		else {
			glEnable(GL_LIGHT0);
			glDisable(GL_LIGHT1);
			darkMode = false;
		}
	}

	// different camera angle
	if (keys['I']) 
	{
		glLoadMatrixf(glm::value_ptr(camera2.GetViewMatrix()));
	}

	// fog
	if (keys['F']) 
	{
		if (!fog) {
			glEnable(GL_FOG); // Enables GL_FOG
			fog = true;
		}

		else {
			glDisable(GL_FOG);
			fog = false;
		}
	}

}
void Skybox(GLfloat size) {

	glBindTexture(GL_TEXTURE_2D, back);

	//back face

	glNormal3f(0.0f, 0.0f, -0.5f);
	glBegin(GL_QUADS);
	glTexCoord2f(1, 0);
	glVertex3f(size, -size, size);

	glTexCoord2f(0, 0);
	glVertex3f(-size, -size, size);

	glTexCoord2f(0, 1);
	glVertex3f(-size, size, size);

	glTexCoord2f(1, 1);
	glVertex3f(size, size, size);
	glEnd();

	//left face

	glNormal3f(0.5f, 0.0f, 0.0f);
	glBindTexture(GL_TEXTURE_2D, left);
	glBegin(GL_QUADS);
	glTexCoord2f(1, 0);
	glVertex3f(-size, -size, size);
	glTexCoord2f(0, 0);
	glVertex3f(-size, -size, -size);
	glTexCoord2f(0, 1);
	glVertex3f(-size, size, -size);
	glTexCoord2f(1, 1);
	glVertex3f(-size, size, size);
	glEnd();


	//front face

	glNormal3f(0.0f, 0.0f, 0.5f);
	glBindTexture(GL_TEXTURE_2D, left);
	glBindTexture(GL_TEXTURE_2D, front);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 1);
	glVertex3f(size, size, -size);
	glTexCoord2f(1, 1);
	glVertex3f(-size, size, -size);
	glTexCoord2f(1, 0);
	glVertex3f(-size, -size, -size);
	glTexCoord2f(0, 0);
	glVertex3f(size, -size, -size);
	glEnd();

	//right face
	glNormal3f(-0.5f, 0.0f, 0.0f);
	glBindTexture(GL_TEXTURE_2D, right);
	glBegin(GL_QUADS);
	glTexCoord2f(1, 0);
	glVertex3f(size, -size, -size);
	glTexCoord2f(0, 0);
	glVertex3f(size, -size, size);
	glTexCoord2f(0, 1);
	glVertex3f(size, size, size);
	glTexCoord2f(1, 1);
	glVertex3f(size, size, -size);
	glEnd();


	//top face

	glNormal3f(0.0f, -0.5f, 0.0f);
	glBindTexture(GL_TEXTURE_2D, top);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex3f(size, size, size);
	glTexCoord2f(0, 1);
	glVertex3f(-size, size, size);
	glTexCoord2f(1, 1);
	glVertex3f(-size, size, -size);
	glTexCoord2f(1, 0);
	glVertex3f(size, size, -size);
	glEnd();


	//DOWN face

	glNormal3f(0.0f, 0.5f, 0.0f);
	glBindTexture(GL_TEXTURE_2D, down);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 1);
	glVertex3f(size, -size, size);
	glTexCoord2f(0, 0);
	glVertex3f(-size, -size, size);
	glTexCoord2f(1, 0);
	glVertex3f(-size, -size, -size);
	glTexCoord2f(1, 1);
	glVertex3f(size, -size, -size);
	glEnd();

}

void build(GLfloat size, GLfloat x, GLfloat z, GLfloat width, GLfloat length, GLfloat Depth, int texture)
{

	glEnable(GL_TEXTURE_2D);

	glPushMatrix();

	glTranslated(size * width - Size + x, LandSize + size * length, size * Depth - Size + z);
	glScaled(width, length, Depth);


	//front face
	glBindTexture(GL_TEXTURE_2D, texture);
	glBegin(GL_QUADS);

	glNormal3f( 0.0f, 0.0f, -1.0f); 
	glTexCoord2f(1, 1);
	glVertex3f(size, size, -size);

	glTexCoord2f(0, 1);
	glVertex3f(-size, size, -size);

	glTexCoord2f(0, 0);
	glVertex3f(-size, -size, -size);

	glTexCoord2f(1, 0);
	glVertex3f(size, -size, -size);
	glEnd();

	
	
	//back face
	glBindTexture(GL_TEXTURE_2D, texture);
	glBegin(GL_QUADS);

	glNormal3f( 0.0f, 0.0f, 1.0f); 
	glTexCoord2f(0, 0);
	glVertex3f(size, -size, size);

	glTexCoord2f(1, 0);
	glVertex3f(-size, -size, size);

	glTexCoord2f(1, 1);
	glVertex3f(-size, size, size);

	glTexCoord2f(0, 1);
	glVertex3f(size, size, size);
	glEnd();

	
	
	//left face
	glBindTexture(GL_TEXTURE_2D, texture);
	glBegin(GL_QUADS);

	glNormal3f( -1.0f, 0.0f, 0.0f); 
	glTexCoord2f(0, 0);
	glVertex3f(-size, -size, size);

	glTexCoord2f(1, 0);
	glVertex3f(-size, -size, -size);

	glTexCoord2f(1, 1);
	glVertex3f(-size, size, -size);

	glTexCoord2f(0, 1);
	glVertex3f(-size, size, size);
	glEnd();

	

	
	//right face
	glBindTexture(GL_TEXTURE_2D, texture);
	glBegin(GL_QUADS);

	glNormal3f( 1.0f, 0.0f, 0.0f); 
	glTexCoord2f(0, 0);
	glVertex3f(size, -size, -size);

	glTexCoord2f(1, 0);
	glVertex3f(size, -size, size);

	glTexCoord2f(1, 1);
	glVertex3f(size, size, size);

	glTexCoord2f(0, 1);
	glVertex3f(size, size, -size);
	glEnd();


	
	//Top face
	glBindTexture(GL_TEXTURE_2D, texture);
	glBegin(GL_QUADS);

	glNormal3f( 0.0f, 1.0f, 0.0f); 
	glTexCoord2f(1, 0);
	glVertex3f(size, size, size);

	glTexCoord2f(0, 0);
	glVertex3f(-size, size, size);

	glTexCoord2f(0, 1);
	glVertex3f(-size, size, -size);

	glTexCoord2f(1 ,1);
	glVertex3f(size, size, -size);
	glEnd();



	//Down face
	glBindTexture(GL_TEXTURE_2D, texture);
	glBegin(GL_QUADS);

	glNormal3f( 0.0f, -1.0f, 0.0f); 
	glTexCoord2f(0, 1);
	glVertex3f(size, -size, size);

	glTexCoord2f(0, 0);
	glVertex3f(-size, -size, size);

	glTexCoord2f(1, 0);
	glVertex3f(-size, -size, -size);

	glTexCoord2f(1, 1);
	glVertex3f(size, -size, -size);
	glEnd();

	glPopMatrix();


}

void Road2(float x1, float z1, float x2, float z2, int Texture) {

	glBindTexture(GL_TEXTURE_2D, Texture);
	
	glBegin(GL_QUADS);

	glTexCoord2f(0, 10);
	glVertex3f(x1, LandSize + 0.1f, z1);

	glTexCoord2f(0, 0);
	glVertex3f(x2, LandSize + 0.1f, z1);

	glTexCoord2f(10, 0);
	glVertex3f(x2, LandSize + 0.1f, z2);

	glTexCoord2f(10, 10);
	glVertex3f(x1, LandSize + 0.1f, z2);

	glEnd();


}

void Road(float x, float z, int Texture)
{

	glBindTexture(GL_TEXTURE_2D, Texture);

	glBegin(GL_POLYGON);

	glTexCoord2d(0, 0);
	glVertex3f(x, LandSize + 0.1f, 0.0);

	glTexCoord2d(1, 0);
	glVertex3f(-x, LandSize + 0.1f, 0.0);


	glTexCoord2d(1, 1);
	glVertex3f(-x, LandSize + 0.1f, z);


	glTexCoord2d(0, 1);
	glVertex3f(x, LandSize + 0.1f, z);


	glEnd();

}


void Wall(float size, float sx, float sy, float sz, float alpha, int texture, GLfloat n) {

	glPushMatrix();
	glTranslated(0, -15 + sy, 0);
	glRotatef(alpha, 0, 1, 0);
	glScalef(sx, sy, -sz);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glBindTexture(GL_TEXTURE_2D, texture);
	//back face
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex3f(size, -size, size);
	glTexCoord2f(n, 0);
	glVertex3f(-size, -size, size);
	glTexCoord2f(n, n);
	glVertex3f(-size, size, size);
	glTexCoord2f(0, n);
	glVertex3f(size, size, size);
	glEnd();

	//left face
	glBindTexture(GL_TEXTURE_2D, texture);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex3f(-size, -size, size);
	glTexCoord2f(n, 0);
	glVertex3f(-size, -size, -size);
	glTexCoord2f(n, n);
	glVertex3f(-size, size, -size);
	glTexCoord2f(0, n);
	glVertex3f(-size, size, size);
	glEnd();

	//front face
	glBindTexture(GL_TEXTURE_2D, texture);
	glBegin(GL_QUADS);
	glTexCoord2f(n, n);
	glVertex3f(size, size, -size);
	glTexCoord2f(0, n);
	glVertex3f(-size, size, -size);
	glTexCoord2f(0, 0);
	glVertex3f(-size, -size, -size);
	glTexCoord2f(n, 0);
	glVertex3f(size, -size, -size);
	glEnd();

	//right face
	glBindTexture(GL_TEXTURE_2D, texture);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex3f(size, -size, -size);
	glTexCoord2f(n, 0);
	glVertex3f(size, -size, size);
	glTexCoord2f(n, n);
	glVertex3f(size, size, size);
	glTexCoord2f(0, n);
	glVertex3f(size, size, -size);
	glEnd();

	//top face
	glBindTexture(GL_TEXTURE_2D, texture);
	glBegin(GL_QUADS);
	glTexCoord2f(n, 0);
	glVertex3f(size, size, size);
	glTexCoord2f(0, 0);
	glVertex3f(-size, size, size);
	glTexCoord2f(0, n);
	glVertex3f(-size, size, -size);
	glTexCoord2f(n, n);
	glVertex3f(size, size, -size);
	glEnd();

	//DOWN face
	glBindTexture(GL_TEXTURE_2D, texture);
	glBegin(GL_QUADS);
	glTexCoord2f(0, n);
	glVertex3f(size, -size, size);
	glTexCoord2f(0, 0);
	glVertex3f(-size, -size, size);
	glTexCoord2f(n, 0);
	glVertex3f(-size, -size, -size);
	glTexCoord2f(n, n);
	glVertex3f(size, -size, -size);
	glEnd();
	glPopMatrix();
}

void Mal() {
	glPushMatrix();
	glTranslatef(-13, 0, 14);
	//glScalef(1.5, 1.5, 1.5);
	//////////////////////////////////////////////////right market
	glPushMatrix();
	glTranslatef(-16, 0, -5);
	for (GLfloat i = 0; i <= 12; i += 4) {
		glPushMatrix();
		glTranslatef(i, 0, 0);
		Wall(1.0f, 3.0f, 3.0f, 0.2f, 90, blackwall, 1);
		glPopMatrix();
	}
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-16, 0, 5);
	for (GLfloat i = 0; i <= 12; i += 4) {
		glPushMatrix();
		glTranslatef(i, 0, 0);
		Wall(1.0f, 3.0f, 3.0f, 0.2f, 90, blackwall, 1);
		glPopMatrix();
	}
	glPopMatrix();
	///////////////////////////////////////////////////// left market
	glPushMatrix();
	glTranslatef(16, 0, -5);
	for (GLfloat i = 0; i <= 12; i += 4) {
		glPushMatrix();
		glTranslatef(-i, 0, 0);
		Wall(1.0f, 3.0f, 3.0f, 0.2f, 90, blackwall, 1);
		glPopMatrix();
	}
	glPopMatrix();

	glPushMatrix();
	glTranslatef(16, 0, 5);
	for (GLfloat i = 0; i <= 12; i += 4) {
		glPushMatrix();
		glTranslatef(-i, 0, 0);
		Wall(1.0f, 3.0f, 3.0f, 0.2f, 90, blackwall, 1);
		glPopMatrix();
	}
	glPopMatrix();





	glPushMatrix();                          //down wall
	glTranslatef(0.0f, -15.2f, 7.0f);
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	Wall(1.0f, 20.0f, 8.0f, 0.2f, 0.0f, whitewall, 5.0f);
	glPopMatrix();

	glPushMatrix();                             // up wall 2
	glTranslatef(0.0f, -3.2f, 7.0f);
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	Wall(1.0f, 20.0f, 8.0f, 0.2f, 0.0f, blackwall, 5.0f);
	glPopMatrix();

	glPushMatrix();                             // up wall 1
	glTranslatef(0.0f, -9.2f, 7.0f);
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	Wall(1.0f, 20.0f, 8.0f, 0.2f, 0.0f, blackwall, 5.0f);
	glPopMatrix();

	glPushMatrix();                                 //front wall
	glTranslatef(0.0f, 0.0f, 8.0f);
	Wall(1.0f, 20.0f, 6.0f, 0.2f, 0.0f, tranwall, 5.0f);
	glPopMatrix();


	glPushMatrix();                                 //right wall
	glTranslatef(-20.0f, 0.0f, 0.0f);
	Wall(1.0f, 8.0f, 6.0f, 0.2f, 90.0f, tranwall, 5.0f);
	glPopMatrix();


	glPushMatrix();                                    //left wall
	glTranslatef(+20.0f, 0.0f, 0.0f);
	Wall(1.0f, 8.0f, 6.0f, 0.2f, 90.0f, tranwall, 5.0f);
	glPopMatrix();


	//glPushMatrix();                                   //back wall
	glTranslatef(0.0f, 0.0f, -8.0f);
	// 
	//glPushMatrix();                                   //right back wall
	//glTranslatef(-11.7,0,0);
	//Wall(1.0, 8.5, 6.0, 0.2, 0, redwall,5);
	//glPopMatrix();
	//
	glPushMatrix();                                   //right column
	glTranslatef(-3.0f ,0.0f ,0.0f);
	Wall(1.0f, 1.5f, 7.0f, 1.0f, 0.0f, tranwall,5.0f);
	glPopMatrix();
	
	glPushMatrix();                                  //left column
	glTranslatef(+3.0f, 0.0f, 0.0f);
	Wall(1.0f, 1.5f, 7.0f, 1.0f, 0.0f, tranwall, 5.0f);
	glPopMatrix();
	//
	//
	//glPushMatrix();                                  //left back wall
	//glTranslatef(+11.7, 0, 0);
	//Wall(1.0, 8.5, 6.0, 0.2, 0, redwall,5);
	//glPopMatrix();
	//
	//glPopMatrix();                                  //finall -8

	//road of mal
	glPushMatrix();
	glTranslatef(-0.5f, 0.0f, -23.0f);
	Road(2.0f, 15.0f, mallRoad);
	glPopMatrix();
	/*
	glPushMatrix();
	glTranslatef(+16.5, 0.1, -15.5);
	Land(-2.0, 0.0, -33, 14, carpark,1);
	glPopMatrix();

	//right land
	glPushMatrix();
	glTranslatef(-16.5, 0.1, -15.5);
	Land(33.0, 0.0, 2.0, 14.0, carpark,1);
	glPopMatrix();
	*/
	glPopMatrix();                                     //finall
}

void Land(float x1, float z1, float x2, float z2,int Texture,GLfloat n) {

	glBindTexture(GL_TEXTURE_2D, Texture);
	//glScalef(x, 1, z);
	glBegin(GL_QUADS);

	glTexCoord2f(0, n);
	glVertex3f(x1, LandSize, z1);

	glTexCoord2f(0, 0);
	glVertex3f(x2, LandSize, z1);

	glTexCoord2f(n, 0);
	glVertex3f(x2, LandSize, z2);

	glTexCoord2f(n, n);
	glVertex3f(x1, LandSize, z2);

	glEnd();


}


void CreatModel(Model_3DS *Model, float x, float y, float z, float scale)
{
	Model->pos.x = x;
	Model->pos.y = LandSize-y;
	Model->pos.z = z;
	Model->scale = scale;
	Model->Draw();
}

void Buildings() {

	GLfloat BuildSize = Size / 15;


	build(BuildSize, 75.0f, 25.0f, 1.0f, 4.5f, 1.0f, Building_1[0]);
	build(BuildSize, 85.0f, 30.0f, 3.0f, 6.1f, 2.0f, Building_1[1]);
	build(BuildSize, 75.0f, 35.0f, 1.0f, 3.4f, 2.0f, Building_1[3]);
    
	build(BuildSize, 82.0f, 60.0f, 2.0f, 4.1f, 1.0f, Building_1[4]);
	build(BuildSize, 74.0f, 70.0f, 2.0f, 7.2f, 2.0f, Building_1[5]);
	build(BuildSize, 80.0f, 90.0f, 2.0f, 5.5f, 1.0f, Building_1[1]);

}


void Park()
{
	glPushMatrix();
	glTranslated(-13, 0, -29);

	

	CreatModel(SLIDING_ZOOLA, -30, -2, -10	, 0.5);
	CreatModel(city_stall, -5.0f, 0.0f, -1.0f, 0.01f);

    for (GLfloat i = 4; i <	20; i += 4)
       CreatModel(TreeModel, -3.0f, 0.0f, i, 0.04f);

	for (GLfloat i = 4; i < 20; i += 4)
		CreatModel(TreeModel, 3.0f, 0.0f, i, 0.04f);
	
	for (GLfloat i = -5; i > -35; i -= 4)
		CreatModel(TreeModel, i, 0.0f, 16.0f, 0.1f);

	for (GLfloat i = 5; i<30; i += 4)
		CreatModel(TreeModel, i, 0.0f, 16.0f, 0.1f);

	for (GLfloat i = 5; i<26; i += 4)
		CreatModel(TreeModel, i, 0.0f, -19.0f, 0.1f);
	

	for (GLfloat i = -4 ; i>-33; i -= 4)
		CreatModel(TreeModel, i, 0.0f, -19.0f, 0.1f);

	Land(-37,-21,33,18,ParkLand,30);
	
	for(GLfloat i = -33.5 ; i<-7 ; i+=6.5)
    CreatModel(fence, i, 0.0f, 17.0f, 0.04f);


	for (GLfloat i = 27.5; i>2; i -= 6.5)
		CreatModel(fence, i, 0.0f, 17.0f, 0.04f);


	CreatModel(grass_block, 30.0f, -0.2f, 17.0f, 0.04f);

	CreatModel(grass_block,  2.2f, -0.2f, 17.0f, 0.04f);
	CreatModel(grass_block, -3.2f, -0.2f, 17.0f, 0.04f);

	Road2(-2.5, -21, 1.5, 18, Sidewalk);

	
	for (GLfloat i = -34; i < -4; i += 8)
	{
		CreatModel(Wood_Bench, i, 0.3f, -1.0f, 0.005f);
		CreatModel(TreeModel,  i, 0.0f, -3.0f,  0.05f);

	}
	for (GLfloat i = 4; i < 34; i += 8)
	{
		CreatModel(Wood_Bench, i, 0.3f, -1.0f, 0.005f);
		CreatModel(TreeModel,  i, 0.0f, -3.0f, 0.05f);
	}
	glPushMatrix();
	glRotated(180.0f, 0.0f, 1.0f, 0.0f);

	for (GLfloat i = 12; i < 34; i += 8)
	{
		CreatModel(Wood_Bench, i, 0.3f, -1.0f, 0.005f);
		CreatModel(TreeModel,  i, 0.0f, -3.0f, 0.05f);
	}
	for (GLfloat i = -10; i > -30; i -= 8)
	{
		CreatModel(Wood_Bench, i, 0.3f, -1.0f, 0.005f);
		CreatModel(TreeModel, i, 0.0f, -3.0f, 0.05f);
	}
	glPopMatrix();
	glPushMatrix();
	glRotated(90.0f, 0.0f, 1.0f, 0.0f);
	

    for (GLfloat i = -14; i < 21; i += 8)
    	CreatModel(TreeModel, i, 0.0f, 30.0f, 0.10f);
	

	for (GLfloat i = -14; i<21; i += 6.5)
		CreatModel(fence, i, 0.0f, 31.0f, 0.04f);
	
	 for (GLfloat i = 5; i < 30; i += 4)
		CreatModel(TreeModel, i, 0.0f, -36.0f, 0.1f);
	

     for (GLfloat i = -4; i > -15; i -= 4)
     		CreatModel(TreeModel, i, 0.0f, -36.0f, 0.10f);
	
	Road2(-2, -37, 2, -2.5, Sidewalk);
	Road2(-2, 1.5, 2, 31, Sidewalk);

	for (GLfloat i = 2; i < 20; i += 8)
	{
		CreatModel(Wood_Bench, i, 0.3f, -2.0f, 0.005f);
		CreatModel(TreeModel, i, 0.0f, -3.0f, 0.05f);
	}

	glPushMatrix();
	glRotated(180.0f, 0.0f, 1.0f, 0.0f);
	for (GLfloat i = -6; i > -20; i -= 8)
	{
		CreatModel(Wood_Bench, i, 0.3f, -1.0f, 0.005f);
		CreatModel(TreeModel, i, 0.0f, -3.0f, 0.05f);
	}
	glPopMatrix();

	glPopMatrix();


	glPopMatrix();


}

void SecondLandDraw() 
{

	Land(20, -50, 50, 50, Sidewalk,30);
	CreatModel(House, 35, 0,-40, 1);

	Buildings();



}

void DrawGLScene(GLvoid)								   	// Here's Where We Do All The Drawing
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	    // Clear Screen And Depth Buffer
	glLoadIdentity();								    	// Reset The Current Modelview Matrix


	// Camera
	POINT p;
	GetPhysicalCursorPos(&p);
	ShowCursor(false);
	mouse_callback((GLfloat)p.x, (GLfloat)p.y);
	glMatrixMode(GL_MODELVIEW);
	glm::mat4 view = camera.GetViewMatrix();
	glLoadMatrixf(glm::value_ptr(view));
	processInput();
	// Camera

	
	inParkShape();

	for (GLfloat i = 47 ; i>-50; i -= 2)
		CreatModel(Street_Model, 20.0f, 0.0f, i, 0.5f);

	glPushMatrix();
		glRotated(90.0f, 0.0f, 1.0f, 0.0f);
		glTranslated(0.0f, 0.0f, 0.8f);
		for (GLfloat i = 16; i>-49; i -= 2)
			CreatModel(Street_Model, 10.0f, 0.0f ,i, 0.5f);
	glPopMatrix();
	
	Skybox(Size);

	Land(-Size,-Size , +Size, +Size, downLand,30);
	SecondLandDraw();

	Park();
	Mal();


	SwapBuffers(hDC);	//DO NOT REMOVE THIS
}
GLvoid KillGLWindow(GLvoid)								// Properly Kill The Window
{
	if (fullscreen)										// Are We In Fullscreen Mode?
	{
		ChangeDisplaySettings(NULL, 0);					// If So Switch Back To The Desktop
		ShowCursor(TRUE);								// Show Mouse Pointer
	}

	if (hRC)											// Do We Have A Rendering Context?
	{
		if (!wglMakeCurrent(NULL, NULL))					// Are We Able To Release The DC And RC Contexts?
		{
			MessageBox(NULL, "Release Of DC And RC Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))						// Are We Able To Delete The RC?
		{
			MessageBox(NULL, "Release Rendering Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		}
		hRC = NULL;										// Set RC To NULL
	}

	if (hDC && !ReleaseDC(hWnd, hDC))					// Are We Able To Release The DC
	{
		MessageBox(NULL, "Release Device Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		hDC = NULL;										// Set DC To NULL
	}

	if (hWnd && !DestroyWindow(hWnd))					// Are We Able To Destroy The Window?
	{
		MessageBox(NULL, "Could Not Release hWnd.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		hWnd = NULL;										// Set hWnd To NULL
	}

	if (!UnregisterClass("OpenGL", hInstance))			// Are We Able To Unregister Class
	{
		MessageBox(NULL, "Could Not Unregister Class.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		hInstance = NULL;									// Set hInstance To NULL
	}
}

/*	This Code Creates Our OpenGL Window.  Parameters Are:					*
*	title			- Title To Appear At The Top Of The Window				*
*	width			- Width Of The GL Window Or Fullscreen Mode				*
*	height			- Height Of The GL Window Or Fullscreen Mode			*
*	bits			- Number Of Bits To Use For Color (8/16/24/32)			*
*	fullscreenflag	- Use Fullscreen Mode (TRUE) Or Windowed Mode (FALSE)	*/

BOOL CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag)
{
	GLuint		PixelFormat;			// Holds The Results After Searching For A Match
	WNDCLASS	wc;						// Windows Class Structure
	DWORD		dwExStyle;				// Window Extended Style
	DWORD		dwStyle;				// Window Style
	RECT		WindowRect;				// Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left = (long)0;			// Set Left Value To 0
	WindowRect.right = (long)width;		// Set Right Value To Requested Width
	WindowRect.top = (long)0;				// Set Top Value To 0
	WindowRect.bottom = (long)height;		// Set Bottom Value To Requested Height

	fullscreen = fullscreenflag;			// Set The Global Fullscreen Flag

	hInstance = GetModuleHandle(NULL);				// Grab An Instance For Our Window
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc = (WNDPROC)WndProc;					// WndProc Handles Messages
	wc.cbClsExtra = 0;									// No Extra Window Data
	wc.cbWndExtra = 0;									// No Extra Window Data
	wc.hInstance = hInstance;							// Set The Instance
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	wc.hbrBackground = NULL;									// No Background Required For GL
	wc.lpszMenuName = NULL;									// We Don't Want A Menu
	wc.lpszClassName = "OpenGL";								// Set The Class Name

	if (!RegisterClass(&wc))									// Attempt To Register The Window Class
	{
		MessageBox(NULL, "Failed To Register The Window Class.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;											// Return FALSE
	}

	if (fullscreen)												// Attempt Fullscreen Mode?
	{
		DEVMODE dmScreenSettings;								// Device Mode
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));	// Makes Sure Memory's Cleared
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);		// Size Of The Devmode Structure
		dmScreenSettings.dmPelsWidth = width;				// Selected Screen Width
		dmScreenSettings.dmPelsHeight = height;				// Selected Screen Height
		dmScreenSettings.dmBitsPerPel = bits;					// Selected Bits Per Pixel
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
		if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
		{
			// If The Mode Fails, Offer Two Options.  Quit Or Use Windowed Mode.
			if (MessageBox(NULL, "The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?", "NeHe GL", MB_YESNO | MB_ICONEXCLAMATION) == IDYES)
			{
				fullscreen = FALSE;		// Windowed Mode Selected.  Fullscreen = FALSE
			}
			else
			{
				// Pop Up A Message Box Letting User Know The Program Is Closing.
				MessageBox(NULL, "Program Will Now Close.", "ERROR", MB_OK | MB_ICONSTOP);
				return FALSE;									// Return FALSE
			}
		}
	}

	if (fullscreen)												// Are We Still In Fullscreen Mode?
	{
		dwExStyle = WS_EX_APPWINDOW;								// Window Extended Style
		dwStyle = WS_POPUP;										// Windows Style
		ShowCursor(FALSE);										// Hide Mouse Pointer
	}
	else
	{
		dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
		dwStyle = WS_OVERLAPPEDWINDOW;							// Windows Style
	}

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

																	// Create The Window
	if (!(hWnd = CreateWindowEx(dwExStyle,							// Extended Style For The Window
		"OpenGL",							// Class Name
		title,								// Window Title
		dwStyle |							// Defined Window Style
		WS_CLIPSIBLINGS |					// Required Window Style
		WS_CLIPCHILDREN,					// Required Window Style
		0, 0,								// Window Position
		WindowRect.right - WindowRect.left,	// Calculate Window Width
		WindowRect.bottom - WindowRect.top,	// Calculate Window Height
		NULL,								// No Parent Window
		NULL,								// No Menu
		hInstance,							// Instance
		NULL)))								// Dont Pass Anything To WM_CREATE
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Window Creation Error.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	static	PIXELFORMATDESCRIPTOR pfd =				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		bits,										// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		16,											// 16Bit Z-Buffer (Depth Buffer)  
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};

	if (!(hDC = GetDC(hWnd)))							// Did We Get A Device Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Can't Create A GL Device Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!(PixelFormat = ChoosePixelFormat(hDC, &pfd)))	// Did Windows Find A Matching Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Can't Find A Suitable PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!SetPixelFormat(hDC, PixelFormat, &pfd))		// Are We Able To Set The Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Can't Set The PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!(hRC = wglCreateContext(hDC)))				// Are We Able To Get A Rendering Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Can't Create A GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!wglMakeCurrent(hDC, hRC))					// Try To Activate The Rendering Context
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Can't Activate The GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	ShowWindow(hWnd, SW_SHOW);						// Show The Window
	SetForegroundWindow(hWnd);						// Slightly Higher Priority
	SetFocus(hWnd);									// Sets Keyboard Focus To The Window
	ReSizeGLScene(width, height);					// Set Up Our Perspective GL Screen

	if (!InitGL())									// Initialize Our Newly Created GL Window
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Initialization Failed.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	return TRUE;									// Success
}

LRESULT CALLBACK WndProc(HWND	hWnd,			// Handle For This Window
	UINT	uMsg,			// Message For This Window
	WPARAM	wParam,			// Additional Message Information
	LPARAM	lParam)			// Additional Message Information
{
	static PAINTSTRUCT ps;


	switch (uMsg)									// Check For Windows Messages
	{

	case WM_PAINT:
		DrawGLScene();
		BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		return 0;

	case WM_TIMER:
		DrawGLScene();
		return 0;

	case WM_ACTIVATE:							// Watch For Window Activate Message
	{
		if (!HIWORD(wParam))					// Check Minimization State
		{
			active = TRUE;						// Program Is Active
		}
		else
		{
			active = FALSE;						// Program Is No Longer Active
		}

		return 0;								// Return To The Message Loop
	}

	case WM_SYSCOMMAND:							// Intercept System Commands
	{
		switch (wParam)							// Check System Calls
		{
		case SC_SCREENSAVE:					// Screensaver Trying To Start?
		case SC_MONITORPOWER:				// Monitor Trying To Enter Powersave?
			return 0;							// Prevent From Happening
		}
		break;									// Exit
	}

	case WM_CLOSE:								// Did We Receive A Close Message?
	{
		PostQuitMessage(0);						// Send A Quit Message
		return 0;								// Jump Back
	}

	case WM_KEYDOWN:							// Is A Key Being Held Down?
	{
		keys[wParam] = TRUE;					// If So, Mark It As TRUE
		return 0;								// Jump Back
	}

	case WM_KEYUP:								// Has A Key Been Released?
	{
		keys[wParam] = FALSE;					// If So, Mark It As FALSE
		return 0;								// Jump Back
	}

	case WM_SIZE:								// Resize The OpenGL Window
	{
		ReSizeGLScene(LOWORD(lParam), HIWORD(lParam));  // LoWord=Width, HiWord=Height
		return 0;								// Jump Back
	}
	}

	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE	hInstance,			// Instance
	HINSTANCE	hPrevInstance,		// Previous Instance
	LPSTR		lpCmdLine,			// Command Line Parameters
	int			nCmdShow)			// Window Show State
{
	MSG		msg;									// Windows Message Structure
	BOOL	done = FALSE;								// Bool Variable To Exit Loop

														// Ask The User Which Screen Mode They Prefer
														//if (MessageBox(NULL,"Would You Like To Run In Fullscreen Mode?", "Start FullScreen?",MB_YESNO|MB_ICONQUESTION)==IDNO)
	{
		fullscreen = FALSE;							// Windowed Mode
	}

	// Create Our OpenGL Window
	if (!CreateGLWindow((char*)"Skybox", 640, 480, 16, fullscreen))
	{
		return 0;									// Quit If Window Was Not Created
	}


	//Set drawing timer to 20 frame per second
	UINT timer = SetTimer(hWnd, 0, 50, (TIMERPROC)NULL);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}


	return 0;

}

void mouse_callback(GLfloat xpos, GLfloat ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = (xpos - lastX) * 7;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLfloat xoffset, GLfloat yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}

void inParkShape() {

	GLUquadric* quadratic = gluNewQuadric();	
	gluQuadricNormals(quadratic, GLU_SMOOTH);		
	gluQuadricTexture(quadratic, GL_TRUE);

	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);

	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP); 


	glBindTexture(GL_TEXTURE_2D, reflectedTex); // This Will Select A Sphere Map

	glPushMatrix();
	glTranslated(-13, LandSize + 2, -29);
	glRotatef(90, 1.0f, 0.0f, 0.0f);

	gluSphere(quadratic, 1.3f, 32, 32);

	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);

	glPopMatrix();

}

