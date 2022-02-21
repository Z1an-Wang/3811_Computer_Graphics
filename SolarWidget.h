#ifndef SOLARWIDGET_H
#define SOLARWIDGET_H
#ifndef __GL_POLYGON_WIDGET_H__
#define __GL_POLYGON_WIDGET_H__ 1

#include <GL/glut.h>
#include <glu.h>
#include <QGLWidget>
#include <vector>

#define SLICES 256
#define STACKS 256

// Define the struct to setup the Material
typedef struct MaterialStruct {
	GLfloat ambient[4];
	GLfloat diffuse[4];
	GLfloat specular[4];
	GLfloat shininess;
} MaterialStruct;

// Define the struct to setup Light
typedef struct LightingStruct {
	GLfloat ambient[4];
	GLfloat diffuse[4];
	GLfloat specular[4];
} LightingStruct;


#define NUM_OBJ 24
#define NUM_PLANET 13
#define NUM_PIC 14
#define BACKGROUND 13

enum planet{
	SUN,
	MERCURY,
	VENUS,
	EARTH,
	MOON,
	MARS,
	MARS_ST1,
	MARS_ST2,
	JUPITER,
	SATURN,
	SATURN_ST,
	URANUS,
	NEPTUNE,
	JUPITER_RING,
	SATURN_RING,
	URANUS_RING,
	ORBIT1,
	ORBIT2,
	ORBIT3,
	ORBIT4,
	ORBIT5,
	ORBIT6,
	ORBIT7,
	ORBIT8
};

class SolarWidget: public QGLWidget {

Q_OBJECT

public:
	// Some Material proporties
	static MaterialStruct whiteShinyMaterials;
	// Some Light proporties
	static LightingStruct sunLighting;
	static LightingStruct universeLighting;

	SolarWidget(QWidget *parent);
	~SolarWidget();

public slots:
	void updateAngleX(int);
	void updateAngleY(int);
	void updateAngleZ(int);
	void updateZoom(int);
	void updateSpeed(int);
	void solarRotate();
	void enableSunLight();
	void enableEnvLight();

protected:
	// called when OpenGL context is set up
	void initializeGL();
	// called every time the widget is resized
	void resizeGL(int w, int h);
	// called every time the widget needs painting
	void paintGL();

private:
	// Settings for the view angle, range, depth
	GLfloat x_angle;
	GLfloat y_angle;
	GLfloat z_angle;
	int viewRange;
	const int VIEWDEPTH = 260;

	// Settings for light
	GLfloat * envLightPos;
	GLfloat * sunLightPos;
	bool enableSun;
	bool enableEnv;

	// Settings for planets object
	std::vector<GLUquadricObj *> starObj;
	const float earthRadius = 5;
	float rotateAngle[NUM_PLANET];
	float rotateRatio = 1.0;

	// Settings for texture.
	std::vector<GLubyte *> pic;
	std::vector<GLint> pic_width;
	std::vector<GLint> pic_height;
	GLuint texture[NUM_PIC];
	const char *filePos[14] = {
		"/Users/prince_an/Lectures/3811_Computer_Graphics/Coursework/Cwk2/Solar_System/textures/sun.bmp",
		"/Users/prince_an/Lectures/3811_Computer_Graphics/Coursework/Cwk2/Solar_System/textures/mercury.bmp",
		"/Users/prince_an/Lectures/3811_Computer_Graphics/Coursework/Cwk2/Solar_System/textures/venus.bmp",
		"/Users/prince_an/Lectures/3811_Computer_Graphics/Coursework/Cwk2/Solar_System/textures/earth.bmp",
		"/Users/prince_an/Lectures/3811_Computer_Graphics/Coursework/Cwk2/Solar_System/textures/moon.bmp",
		"/Users/prince_an/Lectures/3811_Computer_Graphics/Coursework/Cwk2/Solar_System/textures/mars.bmp",
		"/Users/prince_an/Lectures/3811_Computer_Graphics/Coursework/Cwk2/Solar_System/textures/marc.bmp",
		"/Users/prince_an/Lectures/3811_Computer_Graphics/Coursework/Cwk2/Solar_System/textures/marc.bmp",
		"/Users/prince_an/Lectures/3811_Computer_Graphics/Coursework/Cwk2/Solar_System/textures/jupiter.bmp",
		"/Users/prince_an/Lectures/3811_Computer_Graphics/Coursework/Cwk2/Solar_System/textures/saturn.bmp",
		"/Users/prince_an/Lectures/3811_Computer_Graphics/Coursework/Cwk2/Solar_System/textures/marc.bmp",
		"/Users/prince_an/Lectures/3811_Computer_Graphics/Coursework/Cwk2/Solar_System/textures/uranus.bmp",
		"/Users/prince_an/Lectures/3811_Computer_Graphics/Coursework/Cwk2/Solar_System/textures/neptune.bmp",
		"/Users/prince_an/Lectures/3811_Computer_Graphics/Coursework/Cwk2/Solar_System/textures/sky.bmp"};

	/**
	 * Function about drawing the star object in the solar.
	 */
	void drawMercury();
	void drawVenus();
	void drawEarth();
	void drawMars();
	void drawJupiter();
	void drawSaturn();
	void drawUranus();
	void drawNeptune();

	/**
	 * Function for assistance.
	 */
	void loadImage(const char *);
	void initTexture(int);
	void setProjection(double, double, double, double, bool);
	void drawOrbit(GLUquadric *, GLfloat, GLfloat, GLfloat, GLfloat);
	void renderPlanet(int planet, float radius);
	void rotateStep(int planet, float ratio);
};

#endif
#endif