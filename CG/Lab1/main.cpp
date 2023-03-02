#include <iostream>

#include <GL/glut.h>
#include <GL/gl.h>

void windowResize(GLint width, GLint height);
void render();

int main(int argc, char **argv) {
    glutInit(&argc, argv);

    glutInitWindowSize(1280, 720);
    glutInitWindowPosition(120, 80);
    glutCreateWindow("Hello, world");


    glutReshapeFunc(windowResize);
    glutDisplayFunc(render);

    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glClearColor(0.5f, 0.5f, 0.8f, 1.0f);

    glutMainLoop();

    return 0;
}

void render() {
    glClearColor(0.0f, 0.2f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Set color to red
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_LINES);

    glPointSize(3);

    glVertex2d(0.5, 0.25);
    glVertex2d(0.5, 0.5);
    glVertex2d(0.75, 0.5);
    glVertex2d(1, 0.5);

    glEnd();

    glutSwapBuffers();
}

void windowResize(GLint width, GLint height) {
    GLdouble ratio = GLdouble (height) / width;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 1, ratio, 0);

    glClear(GL_COLOR_BUFFER_BIT);
}