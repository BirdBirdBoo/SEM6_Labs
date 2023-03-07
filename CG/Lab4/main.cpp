#include "main.h"

#include <iostream>

#include <GL/glut.h>
#include <GL/gl.h>
#include <cmath>
#include <chrono>
#include <thread>

using namespace std::chrono_literals;

void drawCube(long double timeSinceLastFrameMs);

int main(int argc, char **argv) {
    glutInit(&argc, argv);

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
    glEnable(GL_LIGHT1);

    glEnable(GL_COLOR_MATERIAL);

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glClearColor(0.5f, 0.5f, 0.8f, 1.0f);

    const float lightPos[]{0.f, 10.f, -5.f, 1.f};
    const float specular[]{3.f, 0.f, 3.f, 1};
    const float diffuse[]{3.f, 0.f, 3.f, 10};
    const float ambient[]{0.2f, 0.2f, 0.8f, 1};
    const float direction[]{0.5f, -1.f, -0.5f};
    glLightfv(GL_LIGHT1, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT1, GL_SPECULAR, specular);
    //glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, direction);
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 10);
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 60);
    //glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);

    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);

    glutMainLoop();

    return 0;
}

void render(long double elapsedMs) {
    drawCube(elapsedMs);
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

    glColor3f(0,0,0.6);
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
