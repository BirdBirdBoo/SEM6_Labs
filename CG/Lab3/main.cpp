#include "main.h"

#include <iostream>

#include <GL/glut.h>
#include <GL/gl.h>
#include <cmath>
#include <chrono>
#include <thread>

using namespace std::chrono_literals;

bool usePerspective = true;
bool useWireframes = false;
bool useSmoothShading = true;
float aspectRatio = 1.6;

float surfaceControlPoints[4][4][4]{
        {{0, 0,   0.2, 1}, {.6, 0,   -.35, 1}, {.9,  0,   .6,  1}, {2, 0,   .05, 1}},
        {{0, 0.5, 0.3, 1}, {.5, .5,  -.25, 1}, {1.2, .5,  1.2, 1}, {2, .5,  .05, 1}},
        {{0, 1,   0,   1}, {.6, 1,   .35,  1}, {.9,  1,   .6,  1}, {2, 1,   .45, 1}},
        {{0, 1.5, 0,   1}, {.6, 1.5, -.35, 1}, {.9,  1.5, .6,  1}, {2, 1.5, .45, 1}}
};

float trimCurveControlPoints[5][3]{
        {0.58, 0.23, 1},
        {0.02, 0.09, 2},
        {0.2,  0.6,  1},
        {0.79, 0.36, 1},
        {0.58, 0.23, 1},
};

GLUnurbs *surface;

void drawCube(long double timeSinceLastFrameMs);

void setUpProjection(float ratio);

void onNurbsError(GLenum nErrorCode) {
    char cMessage[128];

    strcpy(cMessage, "NURBS error occured: ");
    auto source = reinterpret_cast<const char *>(gluErrorString((GLenum) nErrorCode));
    strcat(cMessage, source);
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);

    glutInitWindowSize(1280, 720);
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
    glEnable(GL_AUTO_NORMAL);
    glEnable(GL_NORMALIZE);

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glClearColor(0.5f, 0.5f, 0.8f, 1.0f);

    const float lightPos[]{0.f, 10.f, -5.f, 1.f};
    const float specular[]{3.f, 0.f, 3.f, 1};
    const float diffuse[]{.8f, 0.f, .5f, 1};
    const float ambient[]{0.2f, 0.2f, 0.5f, 1};
    const float direction[]{0.5f, -1.f, -0.5f};
    glLightfv(GL_LIGHT1, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT1, GL_SPECULAR, specular);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, direction);
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 6);
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 70);

    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);


    surface = gluNewNurbsRenderer();
    gluNurbsProperty(surface, GLU_DISPLAY_MODE, GL_FILL);
    gluNurbsCallback(surface, GLU_ERROR, (void (__stdcall *)(void)) &onNurbsError);

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

    glClearColor(.8, .8, .8, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glShadeModel(useSmoothShading ? GL_SMOOTH : GL_FLAT);

    auto elapsedDurationNs = duration_cast<std::chrono::nanoseconds>((currentFrameTime - lastFrameTime));

    long double elapsedMs = elapsedDurationNs.count() / 1000000.0;

    render(elapsedMs);

    lastFrameTime = currentFrameTime;

    glutSwapBuffers();

    std::this_thread::sleep_for(1ms);
}

void drawCube(long double timeSinceLastFrameMs) {
    static long double timeMs = 0;

    timeMs += timeSinceLastFrameMs;

    auto timeS = static_cast<float>(timeMs / 1000);

    const float a = 1.5;
    const float xRange = 5;

    float rotationAngle = timeS * 90;

    float x = xRange / 2 + xRange * sinf(.5f * 3.14f * timeS);


    glPushMatrix();

    glTranslatef(x + 1, -3, -10 - a * x);
    glPushMatrix();

    glRotatef(90, 1, 0, 0);
    glRotatef(rotationAngle, 1, 0, 0);
    glTranslatef(0, 0, -2);

    const float redCubeColor[]{0.4f, 0.f, 0.1f, 2.f};

    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glMaterialfv(GL_FRONT, GL_SPECULAR, redCubeColor);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, redCubeColor);
    glMaterialf(GL_FRONT, GL_SHININESS, 6);
    glColor3f(0.8, 0, 0);

    auto cylinder = gluNewQuadric();

    gluQuadricOrientation(cylinder, GLU_OUTSIDE);
    gluQuadricDrawStyle(cylinder, useWireframes ? GLU_LINE : GLU_FILL);
    gluCylinder(cylinder, 2, 2, 4, 16, 1);

    gluQuadricOrientation(cylinder, GLU_INSIDE);
    gluDisk(cylinder, 0, 2, 16, 1);

    glPushMatrix();
    glTranslatef(0, 0, 4);
    gluQuadricOrientation(cylinder, GLU_OUTSIDE);
    gluDisk(cylinder, 0, 2, 16, 1);
    glPopMatrix();

    gluDeleteQuadric(cylinder);

    glPopMatrix();


    glEnable(GL_MAP2_VERTEX_3);

    glPushMatrix();
    glTranslatef(-8, 0, -20);
    glScalef(6, 6, 6);
    glRotatef(rotationAngle, 0, 1, 0);

    glColor3f(0, 1, 0);

    GLfloat edgePt[5][2] = /* counterclockwise */
            {{0.0, 0.0},
             {1.0, 0.0},
             {1.0, 1.0},
             {0.0, 1.0},
             {0.0, 0.0}};

    gluBeginSurface(surface);

    gluNurbsProperty(surface, GLU_DISPLAY_MODE, useWireframes ? GLU_OUTLINE_POLYGON : GLU_FILL);
    gluNurbsProperty(surface, GLU_CULLING, GLU_TRUE);

    float surfaceKnots[] = {0, 0, 0, 0, 1, 1, 1, 1};

    gluNurbsSurface(surface, 8, surfaceKnots, 8, surfaceKnots, 16, 4, reinterpret_cast<float *>(surfaceControlPoints),
                    4, 4,
                    GL_MAP2_VERTEX_4);

    gluBeginTrim(surface);
    gluPwlCurve(surface, 5, &edgePt[0][0], 2, GLU_MAP1_TRIM_2);
    gluEndTrim(surface);

    gluBeginTrim(surface);
    float curveKnots[] = {0, 0, 0, 0, 0, 1, 1, 1, 1, 1};
    gluNurbsCurve(surface, 10, curveKnots, 3, reinterpret_cast<float *>(trimCurveControlPoints), 5, GLU_MAP1_TRIM_3);
    gluEndTrim(surface);


    gluEndSurface(surface);

    glPopMatrix();

    glPopMatrix();
}

void windowResize(GLint newWidth, GLint newHeight) {
    aspectRatio = static_cast<float>(newHeight) / static_cast<float>(newWidth);

    glViewport(0, 0, newWidth, newHeight);

    setUpProjection(aspectRatio);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void setUpProjection(float ratio) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    if (usePerspective) {
        gluPerspective(80, 1 / ratio, 0.05, 1000);
    } else {
        const int size = 20;
        glOrtho(-size, size, -size * ratio, size * ratio, 0.05, 1000);
        gluLookAt(0, 0, 0, 0, 0, -1, 0, 1, 0);
    }

    glMatrixMode(GL_MODELVIEW);
}

void onKeyDown(unsigned char character, int x, int y) {
    switch (character) {
        case 27:        // escape
            exit(0);
        case 'p':
            usePerspective = true;
            setUpProjection(aspectRatio);
            break;
        case 'o':
            usePerspective = false;
            setUpProjection(aspectRatio);
            break;
        case 'f':
            useWireframes = false;
            break;
        case 'w':
            useWireframes = true;
            break;
        case 's':
            useSmoothShading = !useSmoothShading;
            break;
    }
}
