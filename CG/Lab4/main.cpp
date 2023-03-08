#include "main.h"

#include <iostream>

#include <GL/glut.h>
#include <GL/gl.h>
#include <cmath>
#include <chrono>
#include <thread>

using namespace std::chrono_literals;
GLubyte texture_image[64][64][3];

void drawCube(long double timeSinceLastFrameMs);

int main(int argc, char **argv) {
    glutInit(&argc, argv);

    for (int i = 0; i < 64; i++) {
        for (int j = 0; j < 64; j++) {
            if ((i / 4 + j / 4) % 2 == 0) {
                texture_image[i][j][0] = 255; // R component
                texture_image[i][j][1] = 255; // G component
                texture_image[i][j][2] = 255; // B component
            } else {
                texture_image[i][j][0] = 0; // R component
                texture_image[i][j][1] = 0; // G component
                texture_image[i][j][2] = 0; // B component
            }
        }
    }

    glutInitWindowSize(1280, 580);
    glutInitWindowPosition(120, 80);
    glutCreateWindow("Hello, world");

    glutIdleFunc(glutPostRedisplay);
    glutKeyboardFunc(onKeyDown);
    glutReshapeFunc(windowResize);
    glutDisplayFunc(refreshDisplay);

    glEnable(GL_BLEND);
    glEnable(GL_POINT_SMOOTH);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glEnable(GL_LIGHTING);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glEnable(GL_COLOR_MATERIAL);

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glClearColor(0.5f, 0.5f, 0.8f, 1.0f);

    const float lightPos[]{0.f, 20.f, -5.f, 1.f};
    const float specular[]{1.f, 1.f, 0.7f, 1};
    const float diffuse[]{3.f, 0.f, 3.f, 10};
    const float ambient[]{0.2f, 0.2f, 0.2f, 1};
    const float direction[]{0.5f, -1.f, -0.2f};
    glLightfv(GL_LIGHT1, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT1, GL_SPECULAR, specular);
    //glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, direction);
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 16);
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 90);
    //glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);

    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);

    glEnable(GL_LIGHT1);

    glutMainLoop();

    return 0;
}


// Function to draw the cone
void drawCone() {
    // Set the color to green

    const float redCubeColor[]{1.f, 1.f, 1.f};

    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glMaterialfv(GL_FRONT, GL_SPECULAR, redCubeColor);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, redCubeColor);
    glColor3f(1.f, 1.f, 1.f);

    glPushMatrix();
    glRotated(90, -1, 0, 0);

    // Draw the cone
    glutSolidCone(2.0f, 4.0f, 30, 30);
    glPopMatrix();
}

// Function to draw the cylinder
void drawCylinder() {
    // Set the texture for the cylinder
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 64, 64, 0, GL_RGB, GL_UNSIGNED_BYTE, texture_image);


    glPushMatrix();
    glTranslated(-4, 1, 0);
    glRotated(90, 0, 1, 0);


    // Draw the cylinder with texture
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glColor3f(1, 1,1);
    GLUquadricObj *cylinder = gluNewQuadric();
    gluQuadricTexture(cylinder, GL_TRUE);
    gluCylinder(cylinder, 1.0f, 1.0f, 4.0f, 30, 30);
    gluDeleteQuadric(cylinder);
    glDisable(GL_TEXTURE_2D);

    glPopMatrix();
}

void render(long double elapsedMs) {
    //drawCube(elapsedMs);

    glPushMatrix();
    glTranslatef(1, -3, -10);

    drawCone();
    drawCylinder();

    glPopMatrix();
}

void refreshDisplay() {
    static auto currentFrameTime = std::chrono::steady_clock::now();
    static auto lastFrameTime = currentFrameTime;

    currentFrameTime = std::chrono::steady_clock::now();

    glClearColor(0.1, 0.1, 0.1, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto elapsedDurationNs = duration_cast<std::chrono::nanoseconds>((currentFrameTime - lastFrameTime));

    long double elapsedMs = static_cast<long>(elapsedDurationNs.count()) / 1000000.0;

    render(elapsedMs);

    lastFrameTime = currentFrameTime;

    glutSwapBuffers();

    std::this_thread::sleep_for(1ms);
}

void drawCube(long double timeSinceLastFrameMs) {
    static long double timeMs = 0;

    timeMs += timeSinceLastFrameMs;

    auto timeS = static_cast<float >(timeMs / 1000);

    const float a = 1.5;
    const float xRange = 5;

    float rotationAngle = fmodf(timeS * 180, 360);

    float x = xRange * (fmodf(timeS / 2, 1) - 0.5f);


    glPushMatrix();

    glTranslatef(x + 1, -3, -10 - a * x);
    glPushMatrix();
    glRotatef(rotationAngle, 0, 1, 0);

    const float redCubeColor[]{0.4f, 0.f, 0.1f, 2.f};

    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glMaterialfv(GL_FRONT, GL_SPECULAR, redCubeColor);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, redCubeColor);
    glMaterialf(GL_FRONT, GL_SHININESS, 4);
    glColor3f(0.8, 0, 0);

    glutSolidCube(2);
    glPopMatrix();

    glColor3f(0.8, 0, 0.8);
    glTranslated(-4, 0, 0);

    glPushMatrix();
    glRotatef(rotationAngle, 0, 1, 0);
    glutWireCube(3);

    glPopMatrix();

    glColor3f(0, 0, 0.6);
    glTranslated(8, 0, 0);

    glutSolidSphere(2, 32, 32);

    glPopMatrix();
}

void windowResize(GLint newWidth, GLint newHeight) {
    float ratio = static_cast<float>(newHeight) / static_cast<float>(newWidth);

    glViewport(0, 0, newWidth, newHeight);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(80, 1 / ratio, 0.05, 1000);
    gluLookAt(0, 0, 0, 0, 0, -1, 0, 1, 0);

    glMatrixMode(GL_MODELVIEW);


    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void onKeyDown(unsigned char character, int x, int y) {
    switch (character) {
        case 27:        // escape
            exit(0);
    }
}
