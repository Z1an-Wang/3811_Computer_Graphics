#include "SolarWidget.h"
#include <cmath>
#include <iostream>
#include <QDebug>

MaterialStruct SolarWidget::whiteShinyMaterials = {
    {0.6, 0.6, 0.6, 1.0},
    {1.0, 1.0, 1.0, 1.0},
    {0.6, 0.6, 0.6, 1.0},
    60.0};

LightingStruct SolarWidget::sunLighting = {
    {0.2, 0.2, 0.2, 1.0},
    {1.0, 1.0, 1.0, 1.0},
    {0.85, 0.85, 0.85, 1.0}};

LightingStruct SolarWidget::universeLighting = {
    {0.4, 0.4, 0.4, 1.0},
    {0.3, 0.3, 0.3, 1.0},
    {0.10, 0.10, 0.10, 1.0}};

SolarWidget::SolarWidget(QWidget *parent)
    : QGLWidget(parent),x_angle(25.0),y_angle(0.0),z_angle(0.0),viewRange(160) {

    // set the environment light position and enable it
    float initLightPos = 500;
    envLightPos = new GLfloat[4];
    envLightPos[0] = (GLfloat)initLightPos;
    envLightPos[1] = (GLfloat)0;
    envLightPos[2] = (GLfloat)initLightPos;
    envLightPos[3] = (GLfloat)0.0;
    enableEnv = true;

    // set the sun light position and enable it
    sunLightPos = new GLfloat[4];
    sunLightPos[0] = (GLfloat)0.0;
    sunLightPos[1] = (GLfloat)0.0;
    sunLightPos[2] = (GLfloat)0.0;
    sunLightPos[3] = (GLfloat)1.0;
    enableSun = true;

    // init the planet object
    for (int i = 0; i < NUM_OBJ; i++){
        GLUquadricObj *temp = gluNewQuadric();
        starObj.push_back(temp);
    }

    // init rotate angle of each planet as 0
    rotateRatio = 1.0;
    for (int i = 0; i < NUM_PLANET; i++){
        rotateAngle[i] = 0;
    }

    // load the textures from the file
    for (int i = 0; i < NUM_PIC; i++){
        loadImage(filePos[i]);
    }
}

SolarWidget::~SolarWidget(){
    // delete all planet object
    for (int i = 0; i < NUM_OBJ; i++){
        gluDeleteQuadric(starObj[i]);
    }
    starObj.clear();

    pic.clear();
    pic_width.clear();
    pic_height.clear();
    delete[] sunLightPos;
    delete[] envLightPos;
}


/**
 * Function in QGLWidget need to be implement.
 */
void SolarWidget::initializeGL(){
    // set the widget background colour
    glClearColor(0.075, 0.075, 0.075, 0.0);

    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);

    // Load the info about the Material
    glMaterialfv(GL_FRONT, GL_AMBIENT, whiteShinyMaterials.ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, whiteShinyMaterials.diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, whiteShinyMaterials.specular);
    glMaterialf(GL_FRONT, GL_SHININESS, whiteShinyMaterials.shininess);

    // Load info about Light0 - environment light
    glLightfv(GL_LIGHT0, GL_POSITION, envLightPos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, universeLighting.ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, universeLighting.diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, universeLighting.specular);
    glEnable(GL_LIGHT0);

    // Load info about Light1 - sun light
    glLightfv(GL_LIGHT1, GL_POSITION, sunLightPos);
    glLightfv(GL_LIGHT1, GL_AMBIENT, sunLighting.ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, sunLighting.diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, sunLighting.specular);
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 360.);
    glEnable(GL_LIGHT1);

    // Load the texture
    glEnable(GL_TEXTURE_2D);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glGenTextures(NUM_PIC, texture);
    for(int i=0; i<NUM_PIC; i++){
        initTexture(i);
    }
    glDisable(GL_TEXTURE_2D);
}

void SolarWidget::resizeGL(int w, int h){
    // Set the GLWindow as the viewport
    glViewport(0, 0, w, h);

    // Adjust the projection
    setProjection(viewRange, VIEWDEPTH, w, h, false);
}

void SolarWidget::paintGL(){
    // clear the widget
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glShadeModel(GL_SMOOTH);

    // Set the projection base on the window size
    QRect a = this->geometry();
    int w = a.width();
    int h = a.height();
    setProjection(viewRange, VIEWDEPTH, w, h, false);

    // adjust the light
    enableEnv ? glEnable(GL_LIGHT0) : glDisable(GL_LIGHT0);
    enableSun ? glEnable(GL_LIGHT1) : glDisable(GL_LIGHT1);

    // adjust the view angle
    glRotatef(x_angle, 1.0, 0.0, 0.0);
    glRotatef(y_angle, 0.0, 1.0, 0.0);
    glRotatef(z_angle, 0.0, 0.0, 1.0);

    // Render the Sun and set it as an emission material.
    GLfloat emission[4] = {0.75, 0.75, 0.75, 1.0};
    GLfloat emission_clear[4] = {0.0, 0.0, 0.0, 1.0};
    glMaterialfv(GL_FRONT, GL_EMISSION, emission);
    renderPlanet(SUN, 25);

    glMaterialfv(GL_FRONT, GL_EMISSION, emission_clear);

    // Render other planet
    drawEarth();
    drawMercury();
    drawVenus();
    drawMars();
    drawJupiter();
    drawSaturn();
    drawUranus();
    drawNeptune();

    glFlush();
}


/**
 * Function about public slots.
 */
void SolarWidget::updateAngleX(int angleValue){
    this->x_angle = angleValue;
    this->repaint();
}
void SolarWidget::updateAngleY(int angleValue){
    this->y_angle = angleValue;
    this->repaint();
}
void SolarWidget::updateAngleZ(int angleValue){
    this->z_angle = angleValue;
    this->repaint();
}
void SolarWidget::updateZoom(int zoomValue){
    this->viewRange = zoomValue;
    this->repaint();
}
void SolarWidget::updateSpeed(int speedvalue){
    this->rotateRatio = speedvalue/10.0f;
}
void SolarWidget::enableEnvLight(){
    this->enableEnv = !enableEnv;
}
void SolarWidget::enableSunLight(){
    this->enableSun = !enableSun;
}

void SolarWidget::solarRotate(){
    // For easy calculation, we assume there are 360 days a year.
    rotateStep(EARTH, 1.0);

    //The lunar period of revolution is 27.3 days 365/27.3==13.3
    rotateStep(MOON, 13.3);

    //The period of Mercury's revolution is 24.08% of the earth (4.152).
    rotateStep(MERCURY, 4.152);

    //The period of Venus' revolution is 61.56% of the earth (1.62).
    rotateStep(VENUS, 1.62);

    ////The period of Mars' revolution is 200% of the earth.
    rotateStep(MARS, 0.5);
    rotateStep(MARS_ST1, 0.73);
    rotateStep(MARS_ST2, 1.28);

    //The period of Jupiter's revolution is 11.87 times that of the Earth (0.084).
    //For obvious reasons, set it as 0.16
    rotateStep(JUPITER, 0.16);

    //Saturn revolves 29.47 times as long as Earth (0.0339)
    //For obvious reasons, set it as 0.064
    rotateStep(SATURN, 0.064);
    rotateStep(SATURN_ST, 0.64);

    //Uranus revolves 84.06 times as long as Earth (0.012)
    //For obvious reasons, set it as 0.03
    rotateStep(URANUS, 0.03);

    //Neptune revolves 46% longer than Earth.
    rotateStep(NEPTUNE, 0.46);

    this->repaint();
}


/**
 * Function about drawing the star object in the solar.
 */
void SolarWidget::drawMercury(){
    glPushMatrix();

    drawOrbit(starObj[ORBIT1], 30.0, 1.0, 0.0, 0.0);
    glRotatef(rotateAngle[MERCURY], 0.0, 1.0, 0.0);
    glTranslatef(30.0, 0.0, 0.0);                          //The orbit radius of Mercury is 30.0
    renderPlanet(MERCURY, 0.4 * earthRadius);

    glPopMatrix();
}
void SolarWidget::drawVenus(){
    glPushMatrix();

    drawOrbit(starObj[ORBIT2], 40.0, 1.0, 0.0, 0.0);
    glRotatef(rotateAngle[VENUS], 0.0, 1.0, 0.0);
    glTranslatef(40.0, 0.0, 0.0);                           //The orbit radius of Venus is 40.0
    renderPlanet(VENUS, 0.95 * earthRadius);

    glPopMatrix();
}
void SolarWidget::drawEarth(){
    glPushMatrix();

    drawOrbit(starObj[ORBIT3], 55.0, 1.0, 0.0, 0.0);
    glRotatef(rotateAngle[EARTH], 0.0, 1.0, 0.0);
    glTranslatef(55.0, 0.0, 0.0);                           //The orbit radius of the earth is 55.0
    renderPlanet(EARTH, earthRadius);

    glRotatef(rotateAngle[MOON], 0.0, 1.0, 0.0);
    glTranslatef(7.5, 0.0, 0.0);                            //Moon's Orbit radius (to the earth) is 5
    renderPlanet(MOON, 0.27 * earthRadius);

    glPopMatrix();
}
void SolarWidget::drawMars(){
    glPushMatrix();

    drawOrbit(starObj[ORBIT4], 70.0, 1.0, 0.0, 0.0);
    glRotatef(rotateAngle[MARS], 0.0, 1.0, 0.0);
    glTranslatef(70.0, 0.0, 0.0);                           //The orbit radius of the Mars is 70.0
    renderPlanet(MARS, 0.53 * earthRadius);

    //Rotate according to Mars-based coordinates and plot statellite 1 - Phobos
    glPushMatrix();
    glRotatef(rotateAngle[MARS_ST1], 0.0, 1.0, 0.0);
    glTranslatef(4.0, 0.0, 4.0);
    renderPlanet(MARS_ST1, 0.963);
    glPopMatrix();

    //Rotate according to Mars-based coordinates and plot statellite 2 - Deimos
    glPushMatrix();
    glRotatef(rotateAngle[MARS_ST2], 0.0, 1.0, 0.0);
    glTranslatef(-4.5, 0.0, -4.5);
    renderPlanet(MARS_ST2, 1.20);
    glPopMatrix();

    glPopMatrix();
}
void SolarWidget::drawJupiter(){
    glPushMatrix();

    drawOrbit(starObj[ORBIT5], 110.0, 1.0, 0.0, 0.0);
    glRotatef(rotateAngle[JUPITER], 0.0, 1.0, 0.0);
    //The orbit radius of the Jupiter is 110.0
    glTranslatef(110.0, 0.0, 0.0);
    //Jupiter's radius is 11.21 times the radius of Earth, which is 56.05.
    //But for the scale of the sun, take 3.6 times for visual coordination.
    float jupiterRadius = 3.6 * earthRadius;
    renderPlanet(JUPITER, jupiterRadius);

    glPushMatrix();
    //Set the angle of the Jupiter-ring.
    glRotatef(77.0, 1.0, 0.0, 0.0);
    //Define the Jupiter-ring as the gluDisk object and its size is (1-6).
    gluDisk(starObj[JUPITER_RING], jupiterRadius + 1, jupiterRadius + 6, 64, 128);
    glPopMatrix();

    glPopMatrix();
}
void SolarWidget::drawSaturn(){
    glPushMatrix();

    drawOrbit(starObj[ORBIT6], 155.0, 1.0, 0.0, 0.0);
    glRotatef(rotateAngle[SATURN], 0.0, 1.0, 0.0);
    //The orbit radius of the Saturn is 155.0
    glTranslatef(155.0, 0.0, 0.0);
    //Saturn's radius is 9.45 times the radius of Earth, which is 47.25.
    //But for the scale of the sun, take 3 times for visual coordination.
    float saturnRadius = 3.0*earthRadius;
    renderPlanet(SATURN, saturnRadius);

    //Rotate according to Saturn-based coordinates and plot statellite - Mimas
    glPushMatrix();
    glRotatef(rotateAngle[SATURN_ST], 0.0, 1.0, 0.0);
    glTranslatef(-17.0, 0.0, -17.0);
    renderPlanet(SATURN_ST, 1.20);
    glPopMatrix();

    glPushMatrix();
    //Set the angle of the Saturn-ring.
    glRotatef(70.0, 1.0, 0.0, 0.5);
    //Define the Saturn-ring as the gluDisk object and its size is (2-7).
    gluDisk(starObj[SATURN_RING], saturnRadius + 2, saturnRadius + 7, 64, 128);
    glPopMatrix();

    glPopMatrix();
}
void SolarWidget::drawUranus(){
    glPushMatrix();

    drawOrbit(starObj[ORBIT7], 205.0, 1.0, 0.0, 0.0);
    glRotatef(rotateAngle[URANUS], 0.0, 1.0, 0.0);
    //The orbit radius of the Uranus is 205.0
    glTranslatef(205.0, 0.0, 0.0);
    //Here assume Saturn's radius is 2.25 times larger than that of the Earth.
    float uranusRadius = 2.25*earthRadius;
    renderPlanet(URANUS, uranusRadius);

    glPushMatrix();
    //Set the angle of the Saturn-ring.
    glRotatef(150.0, 1.0, 0.0, 1.0);
    //Define the Uranus-ring as the gluDisk object and its size is (1-4).
    gluDisk(starObj[URANUS_RING], uranusRadius + 1, uranusRadius + 4, 64, 128);
    glPopMatrix();

    glPopMatrix();
}
void SolarWidget::drawNeptune(){
    glPushMatrix();

    drawOrbit(starObj[ORBIT8], 250.0, 1.0, 0.0, 0.0);
    glRotatef(rotateAngle[NEPTUNE], 0.0, 1.0, 0.0);
    //The orbit radius of the Neptune is 250.0
    glTranslatef(250.0, 0.0, 0.0);
    //Here assume Neptune's radius is 2 times larger than that of the Earth.
    renderPlanet(NEPTUNE, 2 * earthRadius);

    glPopMatrix();
}


/**
 * Function for assistance.
 */
void SolarWidget::loadImage(const char *path){
    // For 24-bit color map
    static const int BYTESPERPIXEL = 3;
    GLint imgWidth, imgHeight;
    int lineLength, totalLength;
    GLubyte *pixeldata;

    FILE * pfile;
    pfile = fopen(path, "rb");
    if (pfile == 0){exit(0);}

    // Read the size info of the pic from the BMP header.
    fseek(pfile, 0x0012, SEEK_SET);
    fread(&imgWidth, sizeof(imgWidth), 1, pfile);
    fseek(pfile, 0x0016, SEEK_SET);
    fread(&imgHeight, sizeof(imgHeight), 1, pfile);
    pic_width.push_back(imgWidth);
    pic_height.push_back(imgHeight);

    // BMP pic will align the bytes of line in multiples of four.
    lineLength = imgWidth * BYTESPERPIXEL;
    while(lineLength % 4 !=0){
        ++lineLength;
    }
    totalLength = lineLength * imgHeight;

    // Allocate the memory for the pic
    pixeldata = (GLubyte *)malloc(totalLength);
    if (pixeldata == nullptr){exit(0);}

    // read the pic data (skip the first 54bytes as the BMP header)
    fseek(pfile, 54, SEEK_SET);
    int i = fread(pixeldata, sizeof(GLubyte), totalLength, pfile);
    if(i<=1){exit(0);}
    pic.push_back(pixeldata);
    fclose(pfile);
}

void SolarWidget::initTexture(int num){
    // target to which texture is bound and name of a texture
    glBindTexture(GL_TEXTURE_2D, texture[num]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, pic_width[num], pic_height[num],
     0, GL_BGR_EXT, GL_UNSIGNED_BYTE, pic[num]);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    free(pic[num]); pic[num] = nullptr;
}

void SolarWidget::setProjection(double range, double depth, double w, double h, bool con = false){
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    if(con){
        // 透视投影
        GLfloat fAspect = (GLfloat)w / (GLfloat)h;
        gluPerspective((GLdouble)h, fAspect, -depth, depth);
    }
    else {
        // 正交投影
        if (w <= h)
            glOrtho(-range, range, -range * (GLfloat)h / (GLfloat)w, range * (GLfloat)h / (GLfloat)w, -depth, depth);
        else
            glOrtho(-range * (GLfloat)w / (GLfloat)h, range * (GLfloat)w / (GLfloat)h, -range, range, -depth, depth);
    }
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    return;
}

void SolarWidget::drawOrbit(GLUquadric *quad, GLfloat radius, GLfloat dx, GLfloat dy, GLfloat dz){
    glPushMatrix();
    glRotatef(90., dx, dy, dz);
    gluDisk(quad, radius, radius+0.5, 1024, 5);
    glPopMatrix();
}

void SolarWidget::renderPlanet(int planet, float radius)
{
    glPushMatrix();
    glRotatef(-90, 1.0, 0.0, 0.0);

    glBindTexture(GL_TEXTURE_2D, texture[planet]);
    glEnable(GL_TEXTURE_2D);
    gluQuadricTexture(starObj[planet], GL_TRUE);
    gluSphere(starObj[planet], radius, SLICES, STACKS);
    glDisable(GL_TEXTURE_2D);

    glPopMatrix();
}

void SolarWidget::rotateStep(int planet, float ratio){
    rotateAngle[planet] += ratio * rotateRatio;
    if (rotateAngle[planet] >= 360.0)
        rotateAngle[planet] -= 360.0;
}
