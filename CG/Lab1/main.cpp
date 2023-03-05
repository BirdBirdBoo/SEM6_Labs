#include <iostream>

#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/freeglut.h>
#include <cmath>

#define X_MIN (xMin * scale)
#define X_MAX (xMax * scale)

#define Y_MIN (yMin * scale)
#define Y_MAX (yMax * scale)

#define PIXELS_PER_DOT 0.5

constexpr double xMin = -std::numbers::pi, xMax = 3 * std::numbers::pi, yMin = -1.5, yMax = 1.5;
double scale = 1;

double height, width;

void windowResize(GLint width, GLint height);

void render();

void onMouseWheel(int button, int dir, int x, int y);

void drawGrid();

void drawAxis();

void pointsToScreenVertices(double x1, double y1, double x2, double y2);

void pointToScreen(double x, double y, double &screenX, double &screenY);

int floorSigned(double x) {
    return (int) floor(x) + (x < 0 ? 1 : 0);
}

void bitmapString(double x, double y, const char *string);

void drawGraph(double (*func)(double));

int main(int argc, char **argv) {
    glutInit(&argc, argv);

    glutInitWindowSize(1280, 580);
    glutInitWindowPosition(120, 80);
    glutCreateWindow("Hello, world");

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
    drawGraph(std::cos);

    glutSwapBuffers();
}

void drawAxis() {
    double originX, originY;
    pointToScreen(0, 0, originX, originY);

    glLineWidth(2);

    glBegin(GL_LINES);

    glColor3b(0, 0, 0);

    glVertex2d(-width, 0);
    glVertex2d(+width - 4, 0);
    glVertex2d(originX, +height - 4);
    glVertex2d(originX, -height);

    glEnd();

    const double arrowWidth = 20, arrowHeight = 8, arrowReturnOffset = 8;

    glBegin(GL_TRIANGLE_STRIP);

    glVertex2d(+width - arrowWidth, -arrowHeight);
    glVertex2d(+width - arrowWidth + arrowReturnOffset, 0);
    glVertex2d(+width, 0);
    glVertex2d(+width - arrowWidth, +arrowHeight);

    glEnd();

    glBegin(GL_TRIANGLE_STRIP);

    glVertex2d(originX - arrowHeight, height - arrowWidth);
    glVertex2d(originX, height - arrowWidth + arrowReturnOffset);
    glVertex2d(originX, height);
    glVertex2d(originX + arrowHeight, height - arrowWidth);

    glEnd();
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

        pointsToScreenVertices(x, Y_MAX, x, Y_MIN);
    }

    for (int i = minSubunitIndexY; i <= maxSubunitIndexY; ++i) {
        if (i % stepsPerUnitY == 0) {
            continue;
        }

        double y = subunitValueY * i;

        pointsToScreenVertices(X_MAX, y, X_MIN, y);
    }

    glEnd();

    int subunitsPerLabelX = 1, subunitsPerLabelY = 1;

    while ((unitSizeX / stepsPerUnitX) * (2 * subunitsPerLabelX) < labelMinStepSizePixels) {
        subunitsPerLabelX *= 2;
    }

    while ((unitSizeY / stepsPerUnitY) * (2 * subunitsPerLabelY) < labelMinStepSizePixels) {
        subunitsPerLabelY *= 2;
    }

    double screenX, screenY;

    static char label[24];

    glColor3d(0, 0, 0);

    for (int i = minSubunitIndexX; i <= maxSubunitIndexX; ++i) {
        if (i == 0) {
            continue;
        }

        double x = subunitValueX * i;

        if (i % subunitsPerLabelX == 0) {
            std::snprintf(label, 24, "%g", x);

            pointToScreen(x, 0, screenX, screenY);

            bitmapString(screenX + 2, screenY - 16, label);
        }

        if (i % stepsPerUnitX == 0) {
            glBegin(GL_LINES);


            pointsToScreenVertices(x, Y_MAX, x, Y_MIN);

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

            pointToScreen(0, y, screenX, screenY);

            bitmapString(screenX - 22, screenY, label);
        }

        if (i % stepsPerUnitY == 0) {
            glBegin(GL_LINES);

            pointsToScreenVertices(X_MAX, y, X_MIN, y);

            glEnd();
        }
    }


    glEnd();
}

void drawGraph(double (*func)(double)) {
    glLineWidth(6);
    glPointSize(4.5);

    glBegin(GL_POINTS);

    double y;
    double screenX, screenY;

    double stepX = PIXELS_PER_DOT * (X_MAX - X_MIN) / (2 * width);

    for (double x = X_MIN; x <= X_MAX; x += stepX) {
        y = func(x);

        glColor3d(0.3, 0.4 - y * 0.4 / Y_MAX, 0.6 + y * 0.3 / Y_MAX);

        pointToScreen(std::clamp(x, X_MIN, X_MAX), y, screenX, screenY);

        glVertex2d(screenX, screenY);
    }

    glEnd();
}

void windowResize(GLint newWidth, GLint newHeight) {
    width = newWidth / 2.0;
    height = newHeight / 2.0;

    glViewport(0, 0, newWidth, newHeight);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-width, width, -height, height);

    glClear(GL_COLOR_BUFFER_BIT);
}

void pointsToScreenVertices(double x1, double y1, double x2, double y2) {
    double screenX, screenY;

    pointToScreen(x1, y1, screenX, screenY);

    glVertex2d(screenX, screenY);

    pointToScreen(x2, y2, screenX, screenY);

    glVertex2d(screenX, screenY);
}

void pointToScreen(double x, double y, double &screenX, double &screenY) {
    double rangeX = X_MAX - X_MIN, rangeY = Y_MAX - Y_MIN;

    screenX = (x - X_MIN) / rangeX * 2 * width - width;
    screenY = (y - Y_MIN) / rangeY * 2 * height - height;
}

void bitmapString(double x, double y, const char *string) {
    double stringLength = glutBitmapLength(GLUT_BITMAP_HELVETICA_12, reinterpret_cast<const unsigned char *>(string));

    double xOffset = -stringLength / 2;
    glRasterPos2d(x + xOffset, y);

    for (const char *c = string; *c != '\0'; ++c) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
    }
}

void onMouseWheel(int button, int dir, int x, int y) {
    scale *= std::pow(1.2, -dir / 2.0);

    glutPostRedisplay();
}
