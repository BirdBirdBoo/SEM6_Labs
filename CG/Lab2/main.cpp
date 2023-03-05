#include "main.h"

#include <iostream>

#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/freeglut.h>
#include <cmath>
#include <array>

#define X_MIN (xMin * scale)
#define X_MAX (xMax * scale)

#define Y_MIN (yMin * scale)
#define Y_MAX (yMax * scale)

using Point = std::pair<double, double>;

constexpr double xMin = -8.5, xMax = 8.5, yMin = -4, yMax = 4;

auto quad = std::array{
        Point{0.0, 1},
        Point{1.0, 0},
        Point{2.0, 1.5},
        Point{2.5, 0},
};

double scale = 0.7;

double height, width;
double unitsPerPixelX;
double unitsPerPixelY;

void drawQuad();

void glexVertex2p(const Point &point) {
    glVertex2d(point.first, point.second);
}

int floorSigned(double x) {
    return static_cast<int>(floor(x)) + (x < 0 ? 1 : 0);
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);

    glutInitWindowSize(1280, 580);
    glutInitWindowPosition(120, 80);
    glutCreateWindow("Hello, world");

    glutIdleFunc(glutPostRedisplay);
    glutKeyboardFunc(onKeyDown);
    glutMouseWheelFunc(onMouseWheel);
    glutReshapeFunc(windowResize);
    glutDisplayFunc(render);

    glEnable(GL_BLEND);
    glEnable(GL_POINT_SMOOTH);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glClearColor(0.5f, 0.5f, 0.8f, 1.0f);

    glutMainLoop();

    return 0;
}

void render() {
    glClearColor(1, 1, 1, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    drawGrid();
    drawAxis();
    drawQuad();

    glutSwapBuffers();
}

void drawAxis() {
    glLineWidth(2);

    glBegin(GL_LINES);

    glColor3b(0, 0, 0);

    glVertex2d(X_MIN, 0);
    glVertex2d(X_MAX * 0.99, 0);
    glVertex2d(0, Y_MAX * 0.99);
    glVertex2d(0, Y_MIN);

    glEnd();

    const double arrowWidth = 20, arrowHeight = 8, arrowReturnOffset = 8;

    glPushMatrix();

    glTranslated(X_MAX, 0, 0);
    glScaled(unitsPerPixelX, unitsPerPixelY, 1);

    glBegin(GL_TRIANGLE_STRIP);

    glVertex2d(-arrowWidth, -arrowHeight);
    glVertex2d(-arrowWidth + arrowReturnOffset, 0);
    glVertex2d(0, 0);
    glVertex2d(-arrowWidth, +arrowHeight);

    glEnd();

    glPopMatrix();

    glPushMatrix();

    glTranslated(0, Y_MAX, 0);
    glScaled(unitsPerPixelX, unitsPerPixelY, 1);

    glBegin(GL_TRIANGLE_STRIP);

    glVertex2d(-arrowHeight, -arrowWidth);
    glVertex2d(0, -arrowWidth + arrowReturnOffset);
    glVertex2d(0, 0);
    glVertex2d(+arrowHeight, -arrowWidth);

    glEnd();

    glPopMatrix();
}

void drawGrid() {
    double minStepSizePixels = 16;
    double labelMinStepSizePixels = 88;

    int stepsPerUnitX = 1, stepsPerUnitY = 1;

    double unitSizeX = 2 * width / (X_MAX - X_MIN);
    double unitSizeY = 2 * height / (Y_MAX - Y_MIN);

    while (unitSizeX / (2 * stepsPerUnitX) >= minStepSizePixels) {
        stepsPerUnitX *= 2;
    }

    while (unitSizeY / (2 * stepsPerUnitY) >= minStepSizePixels) {
        stepsPerUnitY *= 2;
    }

    double subunitValueX = 1.0 / stepsPerUnitX;
    double subunitValueY = 1.0 / stepsPerUnitY;

    int minSubunitIndexX = floorSigned(X_MIN * stepsPerUnitX);
    int maxSubunitIndexX = floorSigned(X_MAX * stepsPerUnitX);
    int minSubunitIndexY = floorSigned(Y_MIN * stepsPerUnitY);
    int maxSubunitIndexY = floorSigned(Y_MAX * stepsPerUnitY);

    glLineWidth(1);

    glBegin(GL_LINES);
    glColor3d(0.75, 0.75, 0.75);

    for (int i = minSubunitIndexX; i <= maxSubunitIndexX; ++i) {
        if (i % stepsPerUnitX == 0) {
            continue;
        }

        double x = subunitValueX * i;

        glVertex2d(x, Y_MIN);
        glVertex2d(x, Y_MAX);
    }

    for (int i = minSubunitIndexY; i <= maxSubunitIndexY; ++i) {
        if (i % stepsPerUnitY == 0) {
            continue;
        }

        double y = subunitValueY * i;

        glVertex2d(X_MAX, y);
        glVertex2d(X_MIN, y);
    }

    glEnd();

    int subunitsPerLabelX = 1, subunitsPerLabelY = 1;

    while ((unitSizeX / stepsPerUnitX) * (2 * subunitsPerLabelX) < labelMinStepSizePixels) {
        subunitsPerLabelX *= 2;
    }

    while ((unitSizeY / stepsPerUnitY) * (2 * subunitsPerLabelY) < labelMinStepSizePixels) {
        subunitsPerLabelY *= 2;
    }

    static char label[24];

    glColor3d(0, 0, 0);

    for (int i = minSubunitIndexX; i <= maxSubunitIndexX; ++i) {
        if (i == 0) {
            continue;
        }

        double x = subunitValueX * i;

        if (i % subunitsPerLabelX == 0) {
            std::snprintf(label, 24, "%g", x);

            bitmapString(x, 0, 0, -20, label);
        }

        if (i % stepsPerUnitX == 0) {
            glBegin(GL_LINES);

            glVertex2d(x, Y_MAX);
            glVertex2d(x, Y_MIN);

            glEnd();
        }
    }

    for (int i = minSubunitIndexY; i <= maxSubunitIndexY; ++i) {
        if (i == 0) {
            continue;
        }

        double y = subunitValueY * i;

        if (i % subunitsPerLabelY == 0) {
            std::snprintf(label, 24, "%g", y);

            bitmapString(0, y, -20, -4, label);
        }

        if (i % stepsPerUnitY == 0) {
            glBegin(GL_LINES);

            glVertex2d(X_MAX, y);
            glVertex2d(X_MIN, y);

            glEnd();
        }
    }

    glEnd();
}

void drawQuad() {
    const int anchorPoint = 1;

    const double a = 0.5;
    static int counter = 0;
    ++counter;
    double rotationAngle = counter / 2.0;
    double x = ((counter % 1000) - 500) / 100.0;

    glColor4d(0.8, 0.4, 0.4, 0.4);

    glPushMatrix();

    glTranslated(x, a * x, 0);
    glTranslated(quad[anchorPoint].first, quad[anchorPoint].second, 0);
    glRotated(rotationAngle, 0, 0, 1);
    glTranslated(-quad[anchorPoint].first, -quad[anchorPoint].second, 0);

    glBegin(GL_TRIANGLE_STRIP);

    glexVertex2p(quad[0]);
    glexVertex2p(quad[1]);
    glexVertex2p(quad[2]);
    glexVertex2p(quad[3]);

    glEnd();

    glPointSize(8);
    glBegin(GL_POINTS);

    glColor3d(0.8, 0.4, 0.4);
    glexVertex2p(quad[anchorPoint]);

    glEnd();

    glPopMatrix();
}

void windowResize(GLint newWidth, GLint newHeight) {
    width = newWidth / 2.0;
    height = newHeight / 2.0;

    unitsPerPixelX = (X_MAX - X_MIN) / newWidth;
    unitsPerPixelY = (Y_MAX - Y_MIN) / newHeight;

    glViewport(0, 0, newWidth, newHeight);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(X_MIN, X_MAX, Y_MIN, Y_MAX);

    glClear(GL_COLOR_BUFFER_BIT);
}

void bitmapString(double x, double y, double mxPx, double myPx, const char *string) {
    double stringLength = glutBitmapLength(GLUT_BITMAP_HELVETICA_18, reinterpret_cast<const unsigned char *>(string));

    glPushMatrix();
    glTranslated(x, y, 0);
    glScaled(unitsPerPixelX, unitsPerPixelY, 1);
    glTranslated(mxPx - stringLength / 2, myPx, 0);

    glRasterPos2d(0, 0);

    for (const char *c = string; *c != '\0'; ++c) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }

    glPopMatrix();
}

void onMouseWheel([[maybe_unused]] int button, int dir, [[maybe_unused]] int x, [[maybe_unused]] int y) {
    scale *= std::pow(1.2, -dir / 2.0);

    unitsPerPixelX = (X_MAX - X_MIN) / (2 * width);
    unitsPerPixelY = (Y_MAX - Y_MIN) / (2 * height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(X_MIN, X_MAX, Y_MIN, Y_MAX);
}

void onKeyDown(unsigned char character, int x, int y) {
    switch (character) {
        case 27:        // escape
            exit(0);
    }
}
