#include "TextureBuilder.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "GLTexture.h"
#include <string>
#include <glut.h>
#include <Mmsystem.h>
#include <random>
#include <mciapi.h>

int camzoom = 5;
int WIDTH = 1280;
int HEIGHT = 720;
int countenemys = 0;
int timeLeft = 30;
int score = 0;
float pi = 3.141592653589;
double centerzoom = 1.5;
double cameyey = 3.5;

void setupCamera();
void fire();
bool isCollisionHwithB1();
bool isCollisionHwithB2();
bool isCollisionHwithB3();

double cannonBalllaunch = 0.1;
double cannonBallfirerange = 0;
double cooldown = 100;

bool win = false;
bool flagfire = false;
bool flagshot = false;
bool isColRwithE = false;
bool isFirstPCam = false;
bool lose = false;
bool isLevel2 = false;
bool L1 = true;

GLuint tex;
GLuint hero_body;
GLuint villian_body;
GLuint cannonBall_body;
GLTexture tex_ground;
GLTexture tex_floor;
GLTexture tex_building;

GLdouble fovy = 45.0;
GLdouble aspectRatio = (GLdouble)WIDTH / (GLdouble)HEIGHT;
GLdouble zNear = 0.1;
GLdouble zFar = 10000;

class Vector
{
public:
	GLdouble x, y, z;
	Vector() {}
	Vector(GLdouble _x, GLdouble _y, GLdouble _z) : x(_x), y(_y), z(_z) {}
	void operator +=(float value)
	{
		x += value;
		y += value;
		z += value;
	}
};

class Camera {
public:
	Vector eye, center, up;

	Camera(float eyeX = 30.0f, float eyeY = 30.0f, float eyeZ = 30.0f, float centerX = 0.0f, float centerY = 0.0f, float centerZ = 0.0f, float upX = 0.0f, float upY = 1.0f, float upZ = 0.0f) {
		eye = Vector(eyeX, eyeY, eyeZ);
		center = Vector(centerX, centerY, centerZ);
		up = Vector(upX, upY, upZ);
	}

	void look() {
		gluLookAt(
			eye.x, eye.y, eye.z,
			center.x, center.y, center.z,
			up.x, up.y, up.z
			);
	}
};
Camera camera;
void setupCamera() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fovy, (GLdouble)WIDTH / (GLdouble)HEIGHT, zNear, zFar);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	camera.look();
}
Vector eye(30, 10, 30);
Vector center(0, 0, 0);
Vector up(0, 1, 0);

int cameraZoom = 0;

class CannonBall{
public:
	Vector Location;
	Vector Scale;
	Vector Rotationangle;
	double rotangle = 0;
	CannonBall(double locx, double locy, double locz, double scalex, double  scaley, double  scalez, double rotanglex, double rotangley, double rotanglez){
		Location.x = locx;
		Location.y = locy;
		Location.z = locz;

		Scale.x = scalex;
		Scale.y = scaley;
		Scale.z = scalez;

		Rotationangle.x = rotanglex;
		Rotationangle.y = rotangley;
		Rotationangle.z = rotanglez;
	}
	void draw(){
		glPushMatrix();

		GLUquadricObj * qobj5;
		qobj5 = gluNewQuadric();
		glTranslated(Location.x, Location.y, Location.z);
		glRotated(90 + (rotangle*(180 / pi)), Rotationangle.x, Rotationangle.y, Rotationangle.z);
		glScaled(Scale.x, Scale.y, Scale.z);
		glPushMatrix();
		glTranslated(-1.8, 7.5, 0);
		glRotated(-90, 0, 1, 0);
		glColor3f(0.5, 0.5, 0.5);

		gluQuadricTexture(qobj5, true);
		gluQuadricNormals(qobj5, GL_SMOOTH);
		glBindTexture(GL_TEXTURE_2D, cannonBall_body);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		gluSphere(qobj5, 0.5, 30, 30);
		

		glPopMatrix();
		glPopMatrix();

	}
};

class Hero{
public:
	Vector Location;
	Vector Scale;
	Vector Rotationangle;
	double rotangle = 0;
	double angle = 0;
	double angle2 = 0;
	double angle3 = 0;


	Hero(double locx, double locy, double locz, double scalex, double  scaley, double  scalez, double rotanglex, double rotangley, double rotanglez){
		Location.x = locx;
		Location.y = locy;
		Location.z = locz;

		Scale.x = scalex;
		Scale.y = scaley;
		Scale.z = scalez;

		Rotationangle.x = rotanglex;
		Rotationangle.y = rotangley;
		Rotationangle.z = rotanglez;

	}
	void draw(){
		glEnable(GL_TEXTURE_2D);	// Enable 2D texturing
		glPushMatrix();
		glTranslated(Location.x, Location.y, Location.z);
		glRotated(90 + (rotangle*(180 / pi)), Rotationangle.x, Rotationangle.y, Rotationangle.z);
		glScaled(Scale.x, Scale.y, Scale.z);

		glPushMatrix();
		glTranslated(0, sin(angle3), 0);

		glPushMatrix();
		GLUquadricObj * qobj2;
		qobj2 = gluNewQuadric();
		glTranslated(0, 4.5, 0);
		glScaled(1, 1, 1.4);
		glColor3f(1, 1, 1);
		gluQuadricTexture(qobj2, true);
		gluQuadricNormals(qobj2, GL_SMOOTH);
		glBindTexture(GL_TEXTURE_2D, hero_body);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		gluSphere(qobj2, 2, 30, 30);
		glPopMatrix();
		glPushMatrix();
		glTranslated(0, 2, 0);
		glRotated(-90, 1, 0, 0);
		glScaled(1, 1.5, 1);
		glColor3f(0, 0.5, 0.5);
		gluCylinder(qobj2, 0.7, 0.7, 2, 40, 40);
		glPopMatrix();

		glPushMatrix();
		glTranslated(0, 1.5, 0);
		glRotated(-90, 1, 0, 0);
		glColor3f(0.5, 0.5, 0.5);
		glutSolidTorus(0.5, 1, 40, 40);

		glPopMatrix();
		////cannon

		glPushMatrix();
		glTranslated(-1.8, 7.5, 0);
		glRotated(-90, 0, 1, 0);
		glColor3f(0.5, 0.5, 0.5);
		glutSolidTorus(0.2, 0.5, 40, 40);

		glPopMatrix();

		glPushMatrix();
		glTranslated(1.8, 7.5, 0);
		glRotated(-90, 0, 1, 0);
		glColor3f(0.5, 0.5, 0.5);
		glutSolidTorus(0.2, 0.5, 40, 40);

		glPopMatrix();

		glPushMatrix();
		glTranslated(0, 7.5, 0);
		glRotated(-90, 0, 1, 0);
		glColor3f(0.5, 0.5, 0.5);
		glutSolidTorus(0.2, 0.5, 40, 40);

		glPopMatrix();

		glPushMatrix();
		glTranslated(1.7, 7.5, 0);
		glRotated(-90, 0, 1, 0);
		glScaled(1, 1, 1);
		glColor3f(0, 1, 0.5);
		gluCylinder(qobj2, 0.5, 0.5, 3.5, 40, 40);
		glPopMatrix();

		glPushMatrix();
		glTranslated(0, 7.1, 0);
		glRotated(90, 1, 0, 0);
		glScaled(1, 1, 1);
		glColor3f(0, 1, 0.5);
		gluCylinder(qobj2, 0.2, 0.35, 0.8, 40, 40);
		glPopMatrix();
		glPopMatrix();
		glPopMatrix();
	}
};

class Enemy{

public:
	Vector Location;
	Vector Scale;
	Vector Rotationangle;
	double rotangle = 0;
	double transx = 0;
	double transy = 0;
	double transz = 0;
	double angle4 = 0;
	double angle5 = 0;
	Enemy(double locx, double locy, double locz, double scalex, double  scaley, double  scalez, double rotanglex, double rotangley, double rotanglez){
		Location.x = locx;
		Location.y = locy;
		Location.z = locz;

		Scale.x = scalex;
		Scale.y = scaley;
		Scale.z = scalez;

		Rotationangle.x = rotanglex;
		Rotationangle.y = rotangley;
		Rotationangle.z = rotanglez;
	}

	bool lives(){
		if (Scale.x == 0){
			return false;

		}
		else return true;
	}
	void draw(){


		glEnable(GL_TEXTURE_2D);	// Enable 2D texturing
		glPushMatrix();
		glTranslated(Location.x, Location.y, Location.z);
		glRotated(90 + (rotangle*(180 / pi)), Rotationangle.x, Rotationangle.y, Rotationangle.z);
		glScaled(Scale.x, Scale.y, Scale.z);

		glPushMatrix();

		glPushMatrix();
		GLUquadricObj * qobj2;
		qobj2 = gluNewQuadric();
		glTranslated(0, 4.5, 0);
		glScaled(1, 1, 1.4);
		glColor3f(1, 1, 1);
		gluQuadricTexture(qobj2, true);
		gluQuadricNormals(qobj2, GL_SMOOTH);
		glBindTexture(GL_TEXTURE_2D, villian_body);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		gluSphere(qobj2, 2, 30, 30);
		glPopMatrix();
		glPushMatrix();
		glTranslated(0, 2, 0);
		glRotated(-90, 1, 0, 0);
		glScaled(1, 1.5, 1);
		glColor3f(0, 0.5, 0.5);
		gluCylinder(qobj2, 0.7, 0.7, 2, 40, 40);
		glPopMatrix();

		glPushMatrix();
		glTranslated(0, 1.5, 0);
		glRotated(-90, 1, 0, 0);
		glColor3f(0.5, 0.5, 0.5);
		glutSolidTorus(0.5, 1, 40, 40);

		glPopMatrix();
		////cannon

		glPushMatrix();
		glTranslated(-1.8, 7.5, 0);
		glRotated(-90, 0, 1, 0);
		glColor3f(0.5, 0.5, 0.5);
		glutSolidTorus(0.2, 0.5, 40, 40);

		glPopMatrix();

		glPushMatrix();
		glTranslated(1.8, 7.5, 0);
		glRotated(-90, 0, 1, 0);
		glColor3f(0.5, 0.5, 0.5);
		glutSolidTorus(0.2, 0.5, 40, 40);

		glPopMatrix();

		glPushMatrix();
		glTranslated(0, 7.5, 0);
		glRotated(-90, 0, 1, 0);
		glColor3f(0.5, 0.5, 0.5);
		glutSolidTorus(0.2, 0.5, 40, 40);

		glPopMatrix();

		glPushMatrix();
		glTranslated(1.7, 7.5, 0);
		glRotated(-90, 0, 1, 0);
		glScaled(1, 1, 1);
		glColor3f(0, 1, 0.5);
		gluCylinder(qobj2, 0.5, 0.5, 3.5, 40, 40);
		glPopMatrix();

		glPushMatrix();
		glTranslated(0, 7.1, 0);
		glRotated(90, 1, 0, 0);
		glScaled(1, 1, 1);
		glColor3f(0, 1, 0.5);
		gluCylinder(qobj2, 0.2, 0.35, 0.8, 40, 40);
		glPopMatrix();
		glPopMatrix();
		glPopMatrix();
		

	}

}; 

void isCollisionRwithE(Enemy enemy);

class building{
public:
	Vector Location;
	Vector Scale;
	Vector Rotationangle;
	double rotangle = 0;
	building(double locx, double locy, double locz, double scalex, double  scaley, double  scalez, double rotanglex, double rotangley, double rotanglez){
		Location.x = locx;
		Location.y = locy;
		Location.z = locz;

		Scale.x = scalex;
		Scale.y = scaley;
		Scale.z = scalez;

		Rotationangle.x = rotanglex;
		Rotationangle.y = rotangley;
		Rotationangle.z = rotanglez;
	}
	void draw(){
		glEnable(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, tex_building.texture[0]);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glPushMatrix();
		glTranslated(Location.x, Location.y - 0.01, Location.z);
		glRotated(rotangle, Rotationangle.x, Rotationangle.y, Rotationangle.z);
		glScaled(Scale.x, Scale.y, Scale.z);

		glBegin(GL_POLYGON);
		glTexCoord2f(1, 0);
		glVertex3d(1, 0, 1);
		glTexCoord2f(0, 0);
		glVertex3d(0.3, 0, 1);
		glTexCoord2f(0, 1);
		glVertex3d(0.3, 0.4, 1);
		glTexCoord2f(1, 1);
		glVertex3d(1, 0.4, 1);
		glEnd();

		glBindTexture(GL_TEXTURE_2D, tex_floor.texture[0]);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glBegin(GL_POLYGON);
		glTexCoord2f(20, 0);
		glVertex3d(1, 0, 1);
		glTexCoord2f(20, 40);
		glVertex3d(1, 0, -1);
		glTexCoord2f(0, 40);
		glVertex3d(-1, 0, -1);
		glTexCoord2f(0, 0);
		glVertex3d(-1, 0, 1);
		glEnd();

		glPopMatrix();
		
	}
};

void InitLightSource()
{
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT3);

	GLfloat ambient[] = { 0.5f, 0.5f, 0.5, 1.0f };
	GLfloat diffuse[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat specular[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);



	GLfloat ambient3[] = { 1.0f, 0, 0, 1.0f };
	GLfloat diffuse3[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat specular3[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	glLightfv(GL_LIGHT3, GL_DIFFUSE, diffuse3);
	glLightfv(GL_LIGHT3, GL_SPECULAR, specular3);
	glLightfv(GL_LIGHT3, GL_AMBIENT, ambient3);

	GLfloat light_position[] = { 40.0f, 40.0f, 0.0f, 1.0f };

	GLfloat light_position1[] = { -2 - 0.7, 5, -1, 1 };
	GLfloat light_position2[] = { -2 - 0.7, 5, 1, 1 };
	GLfloat light_position3[] = { -1.8 - 0.7, 3.7, 1 };
	glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
	glLightfv(GL_LIGHT1, GL_POSITION, light_position2);
	glLightfv(GL_LIGHT1, GL_POSITION, light_position3);
	GLfloat ambient1[] = { 1, 0, 0, 1.0f };
	GLfloat diffuse1[] = { 1, 0, 0, 1.0f };
	GLfloat specular1[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat light_direction[] = { 0, 1, 0, 1.0f };

	glLightfv(GL_LIGHT1, GL_AMBIENT, ambient1);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse1);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular1);


}

void InitMaterial()
{
	glEnable(GL_COLOR_MATERIAL);

	GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat ambient[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialf(GL_FRONT, GL_SHININESS, 60);

	GLfloat mat_specular[] = { 1.0, 0, 0, 1.0 };
	GLfloat mat_shininess[] = { 50.0 };
	GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);
	GLfloat l1Direction[] = { -1, 0, 0.0 };
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glLightfv(GL_LIGHT1, GL_POSITION, light_position);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 30.0);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 90.0);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, l1Direction);


	GLfloat lightPosition4[] = { 0, 4, 20, 1.0f };
	GLfloat direction[] = { 0, -1, 0.0 };


	glLightfv(GL_LIGHT3, GL_POSITION, lightPosition4);
	glLightf(GL_LIGHT3, GL_SHININESS, 100.0);
	glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, 30.0);
	glLightf(GL_LIGHT3, GL_SPOT_EXPONENT, 90.0);
	glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, direction);
}

void drawText(const char *text, int length, int x, int y){
	glMatrixMode(GL_PROJECTION);
	double *matrix = new double[16];
	glGetDoublev(GL_PROJECTION_MATRIX, matrix);
	glLoadIdentity();
	glOrtho(0, 800, 0, 600, -5, 5);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();
	glLoadIdentity();
	glRasterPos2i(x, y);
	for (int i = 0; i < length; i++){

		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, (int)text[i]);

	}
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixd(matrix);
	glMatrixMode(GL_MODELVIEW);
}
void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	InitLightSource();
	InitMaterial();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
}

void drawGround()
{	 
	glColor3f(0.6, 0.6, 0.6);

	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, tex_ground.texture[0]);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glPushMatrix();
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	glTexCoord2f(0, 0);
	glVertex3f(-200, 0, -200);
	glTexCoord2f(50, 0);
	glVertex3f(200, 0, -200);
	glTexCoord2f(50, 50);
	glVertex3f(200, 0, 200);
	glTexCoord2f(0, 50);
	glVertex3f(-200, 0, 200);
	glEnd();
	glPopMatrix();

	glEnable(GL_LIGHTING);

	glColor3f(1, 1, 1);
}
Hero hero(20, 0.9, 50, 0.3, 0.3, 0.3, 0, 1, 0);
Enemy enemy(10, 0.9, 20, 0.4, 0.4, 0.4, 0, 1, 0);
Enemy enemy1(15, 0.9, 20, 0.4, 0.4, 0.4, 0, 1, 0);
Enemy enemy2(5, 0.9, 20, 0.4, 0.4, 0.4, 0, 1, 0);

Enemy enemy3(10, 0.9, 20, 0.4, 0.4, 0.4, 0, 1, 0);
Enemy enemy4(15, 0.9, 20, 0.4, 0.4, 0.4, 0, 1, 0);
Enemy enemy5(5, 0.9, 20, 0.4, 0.4, 0.4, 0, 1, 0);

building building1(10, 0.1, -5, 10, 10, 40, 1, 1, 1);
building building2(-10, 0.1, -5, 10, 6, 40, 1, 1, 1);
building building3(30, 0.1, -5, 10, 6, 40, 1, 1, 1);
CannonBall cannonBall(hero.Location.x, 1.8, hero.Location.z, 0.2, 0.2, 0.2, 0, 1, 0);
void Display(void)
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (!lose){
		if (!win){
			if (isLevel2 == false) {
				glColor4f(0, 0, 0, 1);
				std::string text;
				text = "Time Left: " + std::to_string(timeLeft) + "                Score: " + std::to_string(score);//score
				drawText(text.data(), text.size(), 20, 20);

				GLfloat lightIntensity[] = { 1, 1, 1, 1 };
				GLfloat lightPosition[] = { 0.0f, 70.0f, 0.0f, 0.0f };
				glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
				glLightfv(GL_LIGHT0, GL_DIFFUSE, lightIntensity);
				glLightfv(GL_LIGHT0, GL_AMBIENT, lightIntensity);
				glLightfv(GL_LIGHT0, GL_SPECULAR, lightIntensity);
				glLightf(GL_LIGHT0, GL_SHININESS, 60.0);
				GLfloat lightIntensity1[] = { 0, 1, 0, 1 };
				GLfloat lightPosition1[] = { hero.Location.x, hero.Location.y, hero.Location.z, 1.0f };
				GLfloat lightPosition2[] = { -2 - 0.7, 5, 1, 1.0f };
				GLfloat lightPosition3[] = { -1.8 - 0.7, 3.7, 0, 1.0f };

				glLightfv(GL_LIGHT1, GL_POSITION, lightPosition1);
				glLightfv(GL_LIGHT1, GL_POSITION, lightPosition2);
				glLightfv(GL_LIGHT1, GL_POSITION, lightPosition3);

				glLightfv(GL_LIGHT1, GL_SPECULAR, lightIntensity1);
				glLightf(GL_LIGHT1, GL_SHININESS, 100.0);

				camera.center.x = hero.Location.x;
				camera.center.z = hero.Location.z;
				camera.center.y = hero.Location.y + centerzoom;
				camera.eye.x = (hero.Location.x) + (camzoom*cosf(hero.rotangle - (0.5*pi)));
				camera.eye.z = hero.Location.z + (camzoom * sinf(hero.rotangle + (0.5*pi)));
				camera.eye.y = cameyey;
				setupCamera();

				hero.draw();
				isCollisionRwithE(enemy);
				if (!isColRwithE) {
					enemy.draw();
				}
				else {
					enemy.Scale.x = 0;
					enemy.Scale.y = 0;
					enemy.Scale.z = 0;
					enemy.Location.x = 1000;
					flagfire = false;
					cannonBallfirerange = 0;
					cooldown = 100;
				}
				isCollisionRwithE(enemy1);
				if (!isColRwithE) {
					enemy1.draw();
				}
				else {
					enemy1.Scale.x = 0;
					enemy1.Scale.y = 0;
					enemy1.Scale.z = 0;
					enemy1.Location.x = 1000;
					flagfire = false;
					cannonBallfirerange = 0;
					cooldown = 100;
				}
				isCollisionRwithE(enemy2);
				if (!isColRwithE) {
					enemy2.draw();
				}
				else {
					enemy2.Scale.x = 0;
					enemy2.Scale.y = 0;
					enemy2.Scale.z = 0;
					enemy2.Location.x = 1000;
					flagfire = false;
					cannonBallfirerange = 0;
					cooldown = 100;
				}

				drawGround();

				building1.draw();
				building2.draw();
				building3.draw();

				if (flagfire == true) {
					if (cannonBallfirerange <= 100) {
						fire();
						cannonBallfirerange += 2;
						cooldown -= 1;
					}
					else {
						flagfire = false;
						cannonBallfirerange = 0;
						cooldown = 100;
					}
				}

				glPushMatrix();
				GLUquadricObj * qobj;
				qobj = gluNewQuadric();
				glTranslated(50, 0, 0);
				glRotated(90, 1, 0, 1);
				glBindTexture(GL_TEXTURE_2D, tex);
				gluQuadricTexture(qobj, true);
				gluQuadricNormals(qobj, GL_SMOOTH);
				gluSphere(qobj, 250, 100, 100);
				gluDeleteQuadric(qobj);


				glPopMatrix();
			}
			else {

				glColor4f(0, 0, 0, 1);
				std::string text;
				text = "Time Left: " + std::to_string(timeLeft) + "                Score: " + std::to_string(score);//score
				drawText(text.data(), text.size(), 20, 20);

				GLfloat lightIntensity[] = { 1, 1, 1, 1 };
				GLfloat lightPosition[] = { 0.0f, 70.0f, 0.0f, 0.0f };
				glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
				glLightfv(GL_LIGHT0, GL_DIFFUSE, lightIntensity);
				glLightfv(GL_LIGHT0, GL_AMBIENT, lightIntensity);
				glLightfv(GL_LIGHT0, GL_SPECULAR, lightIntensity);
				glLightf(GL_LIGHT0, GL_SHININESS, 60.0);
				GLfloat lightIntensity1[] = { 0, 1, 0, 1 };
				GLfloat lightPosition1[] = { hero.Location.x, hero.Location.y, hero.Location.z, 1.0f };
				GLfloat lightPosition2[] = { -2 - 0.7, 5, 1, 1.0f };
				GLfloat lightPosition3[] = { -1.8 - 0.7, 3.7, 0, 1.0f };

				glLightfv(GL_LIGHT1, GL_POSITION, lightPosition1);
				glLightfv(GL_LIGHT1, GL_POSITION, lightPosition2);
				glLightfv(GL_LIGHT1, GL_POSITION, lightPosition3);

				glLightfv(GL_LIGHT1, GL_SPECULAR, lightIntensity1);
				glLightf(GL_LIGHT1, GL_SHININESS, 100.0);

				camera.center.x = hero.Location.x;
				camera.center.z = hero.Location.z;
				camera.center.y = hero.Location.y + centerzoom;
				camera.eye.x = (hero.Location.x) + (camzoom*cosf(hero.rotangle - (0.5*pi)));
				camera.eye.z = hero.Location.z + (camzoom * sinf(hero.rotangle + (0.5*pi)));
				camera.eye.y = cameyey;
				setupCamera();

				hero.draw();
				isCollisionRwithE(enemy3);
				if (!isColRwithE) {
					enemy3.draw();
				}
				else {
					enemy3.Scale.x = 0;
					enemy3.Scale.y = 0;
					enemy3.Scale.z = 0;
					enemy3.Location.x = 1000;
					flagfire = false;
					cannonBallfirerange = 0;
					cooldown = 100;
				}
				isCollisionRwithE(enemy4);
				if (!isColRwithE) {
					enemy4.draw();
				}
				else {
					enemy4.Scale.x = 0;
					enemy4.Scale.y = 0;
					enemy4.Scale.z = 0;
					enemy4.Location.x = 1000;
					flagfire = false;
					cannonBallfirerange = 0;
					cooldown = 100;
				}
				isCollisionRwithE(enemy5);
				if (!isColRwithE) {
					enemy5.draw();
				}
				else {
					enemy5.Scale.x = 0;
					enemy5.Scale.y = 0;
					enemy5.Scale.z = 0;
					enemy5.Location.x = 1000;
					flagfire = false;
					cannonBallfirerange = 0;
					cooldown = 100;
				}

				drawGround();

				building1.draw();
				building2.draw();
				building3.draw();

				if (flagfire == true) {
					if (cannonBallfirerange <= 100) {
						fire();
						cannonBallfirerange += 2;
						cooldown -= 1;
					}
					else {
						flagfire = false;
						cannonBallfirerange = 0;
						cooldown = 100;
					}
				}

				glPushMatrix();
				GLUquadricObj * qobj;
				qobj = gluNewQuadric();
				glTranslated(50, 0, 0);
				glRotated(90, 1, 0, 1);
				glBindTexture(GL_TEXTURE_2D, tex);
				gluQuadricTexture(qobj, true);
				gluQuadricNormals(qobj, GL_SMOOTH);
				gluSphere(qobj, 250, 100, 100);
				gluDeleteQuadric(qobj);


				glPopMatrix();

			

			}
		}
		else {
			glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
			glColor4f(0, 0, 0, 1);
			std::string text;
			text = "YOU WON! Your Score is: " + std::to_string(score);
			drawText(text.data(), text.size(), 320, 320);
		}
	}
	else {
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glColor4f(1, 1, 1, 1);
		std::string text;
		text = "YOU LOST! Your Score is: " + std::to_string(score);
		drawText(text.data(), text.size(), 320, 320);
	}
	glutSwapBuffers();
}
bool isCollisionHwithE(Enemy enemy){
	if (hero.Location.x <= enemy.Location.x + 1 && hero.Location.x >= enemy.Location.x - 1){
		if (hero.Location.z <= enemy.Location.z + 1 && hero.Location.z >= enemy.Location.z - 1){
			return true;
		}
		else return false;
	}
	else return false;
}
bool flag;
bool flag2;
void LoadAssets();
void Anim() {
	if (!enemy.lives() && !enemy1.lives() && !enemy2.lives() &&
		 L1) {
		isLevel2 = true;
		LoadAssets();
		L1 = false;
	}
	
	if (!enemy3.lives() && !enemy4.lives() &&
		!enemy5.lives() && isLevel2) {
		win = true;
	}

}
void timer(int value){

	timeLeft -= 1;
	if (timeLeft == 0 && !win)
		lose = true;
	glutTimerFunc(1000, timer, 0);
	glutPostRedisplay();
}
void Keyboard(unsigned char button, int x, int y)
{
	switch (button)
	{
	case 'w':
		hero.Location.z = (hero.Location.z) - (0.2*cosf(hero.rotangle));
		hero.Location.x = (hero.Location.x) - (0.2*sinf(hero.rotangle));
		if (isCollisionHwithB1() || isCollisionHwithB2() || isCollisionHwithB3() || isCollisionHwithE(enemy) || isCollisionHwithE(enemy1) || isCollisionHwithE(enemy2)){
			hero.Location.z = (hero.Location.z) + (0.2*cosf(hero.rotangle));
			hero.Location.x = (hero.Location.x) + (0.2*sinf(hero.rotangle));
		}
		if (flag == false){
			hero.angle += 5;
			if (hero.angle == 45){
				flag = true;
			}
		}
		else if (flag == true){
			hero.angle -= 5;
			if (hero.angle == -45){
				flag = false;
			}
		}

		if (flag2 == false){
			hero.angle2 -= 5;
			if (hero.angle2 == -45){
				flag2 = true;
			}
		}
		else if (flag2 == true){
			hero.angle2 += 5;
			if (hero.angle2 == 45){
				flag2 = false;
			}
		}
		if (isFirstPCam == false){
			hero.angle3 += 0.223;
			if (hero.angle3 == 90){
				hero.angle3 = 0;
			}

		}
		else
			hero.angle3 = 30;
		break;
	case 's':

		hero.Location.z = (hero.Location.z) + (0.2*cosf(hero.rotangle));
		hero.Location.x = (hero.Location.x) + (0.2*sinf(hero.rotangle));
		if (isCollisionHwithB1() || isCollisionHwithB2() || isCollisionHwithB3() || isCollisionHwithE(enemy) || isCollisionHwithE(enemy1) || isCollisionHwithE(enemy2)){
			hero.Location.z = (hero.Location.z) - (0.2*cosf(hero.rotangle));
			hero.Location.x = (hero.Location.x) - (0.2*sinf(hero.rotangle));
		}
		if (flag == false){
			hero.angle += 5;
			if (hero.angle == 45){
				flag = true;
			}
		}
		else if (flag == true){
			hero.angle -= 5;
			if (hero.angle == -45){
				flag = false;
			}
		}

		if (flag2 == false){
			hero.angle2 -= 5;
			if (hero.angle2 == -45){
				flag2 = true;
			}
		}
		else if (flag2 == true){
			hero.angle2 += 5;
			if (hero.angle2 == 45){
				flag2 = false;
			}
		}
		if (isFirstPCam == false){
			hero.angle3 += 0.223;
			if (hero.angle3 == 90){
				hero.angle3 = 0;
			}
		}
		else
			hero.angle3 = 30;
		break;
	case 'd':

		hero.Location.z = (hero.Location.z) - (0.2*cosf(hero.rotangle - (0.5*pi)));
		hero.Location.x = (hero.Location.x) - (0.2*sinf(hero.rotangle - (0.5*pi)));
		if (isCollisionHwithB1() || isCollisionHwithB2() || isCollisionHwithB3() || isCollisionHwithE(enemy) || isCollisionHwithE(enemy1) || isCollisionHwithE(enemy2)){
			hero.Location.z = (hero.Location.z) - (0.2*cosf(hero.rotangle + (0.5*pi)));
			hero.Location.x = (hero.Location.x) - (0.2*sinf(hero.rotangle + (0.5*pi)));
		}
		break;
	case 'a':

		hero.Location.z = (hero.Location.z) - (0.2*cosf(hero.rotangle + (0.5*pi)));
		hero.Location.x = (hero.Location.x) - (0.2*sinf(hero.rotangle + (0.5*pi)));
		if (isCollisionHwithB1() || isCollisionHwithB2() || isCollisionHwithB3() || isCollisionHwithE(enemy) || isCollisionHwithE(enemy1) || isCollisionHwithE(enemy2)){
			hero.Location.z = (hero.Location.z) - (0.2*cosf(hero.rotangle - (0.5*pi)));
			hero.Location.x = (hero.Location.x) - (0.2*sinf(hero.rotangle - (0.5*pi)));
		}
		break;
	case 'x':
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;
	case 'z':
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;

	case 'c':
		if (camzoom == 5) camzoom = 1;
		else camzoom = 5;

		if (centerzoom == 1.5) centerzoom = 2;
		else centerzoom = 1.5;
		if (cameyey == 3.5) cameyey = 3.2;
		else cameyey = 3.5;

		isFirstPCam = !isFirstPCam;
		break;

	case 'q':
		hero.Location.z = (hero.Location.z) - (0.8*cosf(hero.rotangle));
		hero.Location.x = (hero.Location.x) - (0.8*sinf(hero.rotangle));
		if (isCollisionHwithB1() || isCollisionHwithB2() || isCollisionHwithB3() || isCollisionHwithE(enemy) || isCollisionHwithE(enemy1) || isCollisionHwithE(enemy2)){
			hero.Location.z = (hero.Location.z) + (0.8*cosf(hero.rotangle));
			hero.Location.x = (hero.Location.x) + (0.8*sinf(hero.rotangle));
		}
		if (flag == false){
			hero.angle += 5;
			if (hero.angle == 45){
				flag = true;
			}
		}
		else if (flag == true){
			hero.angle -= 5;
			if (hero.angle == -45){
				flag = false;
			}
		}

		if (flag2 == false){
			hero.angle2 -= 5;
			if (hero.angle2 == -45){
				flag2 = true;
			}
		}
		else if (flag2 == true){
			hero.angle2 += 5;
			if (hero.angle2 == 45){
				flag2 = false;
			}
		}
		hero.angle3 += 0.223;
		if (hero.angle3 == 90){
			hero.angle3 = 0;
		}
		break;

	case 27:
		exit(0);
		break;
	default:
		break;
	}

	glutPostRedisplay();
}

double prex = 640;
double prey = 360;

void Mouse(int x, int y){
	if (x > 720){
		if (x >= prex)
			hero.rotangle -= 0.1;

	}

	else if (x < 560){
		if (x <= prex)
			hero.rotangle += 0.1;
	}
	if (isFirstPCam == false){
		if (y > 440){
			if (y >= prey)
				centerzoom -= 0.1;
		}

		else if (y < 280){
			if (y <= prey)
				centerzoom += 0.1;

		}
	}
	else centerzoom = 2;
	prey = y;
	prex = x;
	glutPostRedisplay();
}

bool isCollisionHwithB1(){

		if (hero.Location.x <= building1.Location.x + 10 && hero.Location.x >= building1.Location.x - 10)
		{
			if ((hero.Location.z <= building1.Location.z + 40.5  && hero.Location.z >= building1.Location.z + 39.5))
			{
				if (((hero.Location.x <= building1.Location.x + 30.5 && hero.Location.x >= building1.Location.x + 2.5)))
					return true;
				else
					return false;
			}
			else if (hero.Location.z >= building1.Location.z - 40.5  && hero.Location.z <= building1.Location.z - 39.5)
				return true;
			else
				return false;
		}
		else
			return false;


}
bool isCollisionHwithB2() {//left

	if (hero.Location.x <= building2.Location.x + 10 && hero.Location.x >= building2.Location.x + 4.5)
	{
		if ((hero.Location.z <= building2.Location.z + 40.5  && hero.Location.z >= building2.Location.z + 39.5))
		{
			if (((hero.Location.x <= building2.Location.x + 30.5 && hero.Location.x >= building2.Location.x + 2.5)))
				return true;
			else
				return false;
		}
		else if (hero.Location.z >= building2.Location.z - 40.5  && hero.Location.z <= building2.Location.z - 39.5)
			return true;
		else
			return false;
	}
	else
		return false;


}
bool isCollisionHwithB3() {//right

	if (hero.Location.x <= building3.Location.x + 30.5 && hero.Location.x >= building3.Location.x - 30.5)
	{
		if ((hero.Location.z <= building3.Location.z + 40.5  && hero.Location.z >= building3.Location.z + 39.5))
		{
			if (((hero.Location.x <= building3.Location.x + 30.5 && hero.Location.x >= building3.Location.x + 2.5)))
				return true;
			else
				return false;
		}
		else if (hero.Location.z >= building3.Location.z - 40.5  && hero.Location.z <= building3.Location.z - 39.5)
			return true;
		else
			return false;
	}
	else
		return false;


}
void isCollisionRwithE(Enemy enemy){
	if (cannonBall.Location.x <= enemy.Location.x + 1 && cannonBall.Location.x >= enemy.Location.x - 1){
		if (cannonBall.Location.z <= enemy.Location.z + 1 && cannonBall.Location.z >= enemy.Location.z - 1){
			sndPlaySound("Sound/Exploding.wav", SND_ASYNC);
			isColRwithE = true;
			score += 50;
		}
		else isColRwithE = false;
	}
	else isColRwithE = false;
}
void LoadAssets()
{
	loadBMP(&hero_body, "Textures/tank.bmp", true);
	loadBMP(&cannonBall_body, "Textures/cannon.bmp", true);
	tex_floor.Load("Textures/water.bmp");

	if (isLevel2 == false)
	{
		tex_building.Load("Textures/Wood.bmp");
		tex_ground.Load("Textures/sand.bmp");
		loadBMP(&villian_body, "Textures/enemy.bmp", true);
		loadBMP(&tex, "Textures/sky4-jpg2.bmp", true);
	}
	else
	{
		tex_building.Load("Textures/stonewall.bmp");
		tex_ground.Load("Textures/grass.bmp");
		loadBMP(&villian_body, "Textures/enemy.bmp", true);
		loadBMP(&tex, "Textures/nightsky.bmp", true);
	}
}

void fire(){
	if (flagshot == false){
		cannonBall.Location.z = ((hero.Location.z) - (cannonBalllaunch*cosf(hero.rotangle)));
		cannonBall.Location.x = ((hero.Location.x) - (cannonBalllaunch*sinf(hero.rotangle)));
		cannonBall.rotangle = hero.rotangle;
	}
	flagshot = true;
	cannonBall.Location.z = ((cannonBall.Location.z) - (cannonBalllaunch*cosf(cannonBall.rotangle)));
	cannonBall.Location.x = ((cannonBall.Location.x) - (cannonBalllaunch*sinf(cannonBall.rotangle)));
	cannonBalllaunch += 0.1;
	cannonBall.draw();
	glutPostRedisplay();
}

void MouseClick(int button, int state, int x, int y){
	if (button == GLUT_LEFT_BUTTON){
		if (cooldown == 100){
			flagfire = true;
			flagshot = false;
			cannonBalllaunch = 0;
			sndPlaySound("Sound/Cannon.wav", SND_ASYNC);
		}
	}

	glutPostRedisplay();
}
void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutInitWindowPosition(100, 80);
	glutCreateWindow("Cannon Hopper");
	mciSendString("play Sound/intro.mp3", NULL, 0, NULL);
	glutIdleFunc(Anim);
	glutTimerFunc(0, timer, 0);
	glutDisplayFunc(Display);
	glutKeyboardFunc(Keyboard);
	glutPassiveMotionFunc(Mouse);
	glutMouseFunc(MouseClick);

	init();

	LoadAssets();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	
	
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_COLOR);
	glShadeModel(GL_SMOOTH);

	glutMainLoop();
}