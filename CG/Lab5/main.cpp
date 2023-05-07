#include <iostream>

#include <GL/glut.h>
#include <GL/gl.h>
#include <cmath>
#include <chrono>
#include <thread>


using namespace std::chrono_literals;
GLuint checkerboardTexture;

typedef std::array<float, 3> vec3;

const float
        xLimit = 11.f,
        yLimit = 7.5f,
        zLimitMin = -25.f,
        zLimitMax = -5.f;

const int
        ITEM_INCREASE_SPEED = 0,
        ITEM_DECREASE_SPEED = 1,
        ITEM_TOGGLE_LIGHT_1 = 2,
        ITEM_TOGGLE_LIGHT_2 = 4,
        ITEM_TOGGLE_LIGHT_3 = 8,
        ITEM_TOGGLE_LIGHT_4 = 16;

const auto menuItems = {
        std::make_pair(ITEM_INCREASE_SPEED, "Increase speed"),
        std::make_pair(ITEM_DECREASE_SPEED, "Decrease speed"),
        std::make_pair(ITEM_TOGGLE_LIGHT_1, "Toggle spotlight"),
        std::make_pair(ITEM_TOGGLE_LIGHT_2, "Toggle far light"),
        std::make_pair(ITEM_TOGGLE_LIGHT_3, "Toggle ambient light"),
        std::make_pair(ITEM_TOGGLE_LIGHT_4, "Toggle point light"),
};

const float animationSpeedIncrementFactor = 1.2f;

int enabledLights = ITEM_TOGGLE_LIGHT_1;
int fogIndex = 0;

vec3 coneTranslation;
vec3 coneSpeed;

float coneRotationAngle = 0;
float coneRotationSpeed = 80;

float animationSpeed = 1;


void initCheckerboardTexture();

void windowResize(int width, int height);

void refreshDisplay();

void onKeyDown(unsigned char character, int x, int y);

void onMenuActivated(int index);

void setLightEnabled(int lightIndex, bool enabled) {
    if (enabled) {
        glEnable(GL_LIGHT1 + lightIndex);
    } else {
        glDisable(GL_LIGHT1 + lightIndex);
    }
}

void updateLights() {
    setLightEnabled(0, enabledLights & ITEM_TOGGLE_LIGHT_1);
    setLightEnabled(1, enabledLights & ITEM_TOGGLE_LIGHT_2);
    setLightEnabled(2, enabledLights & ITEM_TOGGLE_LIGHT_3);
    setLightEnabled(3, enabledLights & ITEM_TOGGLE_LIGHT_4);
}

void applySpeed(vec3 &position, const vec3 &speed, float timeSeconds) {
    position[0] += static_cast<float>(speed[0] * timeSeconds * animationSpeed);
    position[1] += static_cast<float>(speed[1] * timeSeconds * animationSpeed);
    position[2] += static_cast<float>(speed[2] * timeSeconds * animationSpeed);
}

void checkPositionAndBounce(const vec3 &position, vec3 &speed) {
    if (position[0] < -xLimit || position[0] > xLimit) {
        speed[0] = -speed[0];
    }

    if (position[1] < -yLimit || position[1] > yLimit) {
        speed[1] = -speed[1];
    }

    if (position[2] < zLimitMin || position[2] > zLimitMax) {
        speed[2] = -speed[2];
    }
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);

    coneTranslation = {0.f, 0.f, -12.f};
    coneSpeed = {4.f, -2.f, 10.f};

    glutInitWindowSize(1280, 580);
    glutInitWindowPosition(120, 80);
    glutCreateWindow("Hello, world");

    glutIdleFunc(glutPostRedisplay);
    glutKeyboardFunc(onKeyDown);
    glutReshapeFunc(windowResize);
    glutDisplayFunc(refreshDisplay);

    glutCreateMenu(onMenuActivated);
    for (auto item: menuItems) {
        glutAddMenuEntry(item.second, item.first);
    }
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glEnable(GL_BLEND);
    glEnable(GL_POINT_SMOOTH);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glEnable(GL_LIGHTING);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glClearColor(0.5f, 0.5f, 0.8f, 1.0f);

    const float lightPos[]{-3.f, 12.f, 5.f, 1.f};
    const float direction[]{0.2f, -1.f, -1.f};
    const float specular[]{0.4f, .4f, 0.4f, 1};
    const float diffuse[]{0.4f, .4f, 0.4f, 1};

    glLightfv(GL_LIGHT1, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT1, GL_SPECULAR, specular);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, direction);
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 2);
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 8);

    const float lightPosFar[]{-300.f, 200, 0, 1.f};
    const float diffuseFar[]{0.4f, .4f, 0.4f, 1};
    const float specularFar[]{0.1f, .1f, 0.1f, 1};

    glLightfv(GL_LIGHT2, GL_POSITION, lightPosFar);
    glLightfv(GL_LIGHT2, GL_SPECULAR, specularFar);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, diffuseFar);

    const float ambient[]{0.15f, 0.15f, 0.15f, 1};
    glLightfv(GL_LIGHT3, GL_AMBIENT, ambient);

    const float lightPosSpot[]{6.f, 8.f, 0.f, 1.f};
    const float diffuseSpot[]{0.6f, .4f, 0.4f, 1};
    const float specularSpot[]{0.1f, .1f, 0.1f, 1};

    glLightfv(GL_LIGHT4, GL_POSITION, lightPosSpot);
    glLightfv(GL_LIGHT4, GL_SPECULAR, specularSpot);
    glLightfv(GL_LIGHT4, GL_DIFFUSE, diffuseSpot);

    const float noAmbientLight[]{0, 0, 0, 1};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, noAmbientLight);

    glDisable(GL_LIGHT1);    // spot
    glDisable(GL_LIGHT2);    // far
    glDisable(GL_LIGHT3);    // ambient
    glDisable(GL_LIGHT4);    // point


    initCheckerboardTexture();

    glutFullScreen();

    glutMainLoop();

    return 0;
}

void drawCone() {
    const float redColorDiffuse[]{1.f, 0.f, 0.f};
    const float redColorSpecular[]{1.f, .3f, .1f};

    glEnable(GL_COLOR_MATERIAL);

    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glMaterialfv(GL_FRONT, GL_SPECULAR, redColorSpecular);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, redColorDiffuse);
    glColor3f(1, 0, 0);

    glPushMatrix();
    glRotated(90, -1, 0, 0);

    glutSolidCone(2.0f, 4.0f, 64, 64);
    glPopMatrix();

    glDisable(GL_COLOR_MATERIAL);
}

void drawCylinder() {
    glColor3f(1, 1, 1);

    glEnable(GL_COLOR_MATERIAL);

    glPushMatrix();
    glTranslated(-4, 1, 0);
    glRotated(90, 0, 1, 0);

    glBindTexture(GL_TEXTURE_2D, checkerboardTexture);

    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    GLUquadricObj *cylinder = gluNewQuadric();
    gluQuadricTexture(cylinder, GL_TRUE);
    gluCylinder(cylinder, 1.0f, 1.0f, 4.0f, 48, 48);
    gluDeleteQuadric(cylinder);
    glDisable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, -1);

    glPushMatrix();

    gluQuadricOrientation(cylinder, GLU_OUTSIDE);
    gluDisk(cylinder, 0, 1, 48, 8);

    glPopMatrix();

    glPopMatrix();

    glDisable(GL_COLOR_MATERIAL);
}

void render(float totalTimeSeconds, long double frameTimeMs) {
    static const float fogColor[]{.5f, .6f, .7f};

    glEnable(GL_FOG);
    glFogi(GL_FOG_MODE, fogIndex == 0 ? GL_LINEAR : (GL_EXP + fogIndex % 2));
    glFogf(GL_FOG_START, 8);
    glFogf(GL_FOG_END, 40);
    glFogf(GL_FOG_DENSITY, 0.04);
    glFogfv(GL_FOG_COLOR, fogColor);

    updateLights();

    glPushMatrix();

    glTranslatef(coneTranslation[0], coneTranslation[1], coneTranslation[2]);

    glRotatef(coneRotationAngle, 0, 1, 0);

    drawCone();
    drawCylinder();

    glPopMatrix();

    applySpeed(coneTranslation, coneSpeed, static_cast<float>(frameTimeMs) / 1000);
    checkPositionAndBounce(coneTranslation, coneSpeed);

    coneRotationAngle += coneRotationSpeed * static_cast<float>(frameTimeMs) / 1000 * animationSpeed;
    coneRotationAngle = fmodf(coneRotationAngle, 360);

    std::cout << "Frame time: " << frameTimeMs << " ms, "
              << "Speed: (" << coneSpeed[0] << ", " << coneSpeed[1] << ", " << coneSpeed[2] << "), "
              << "Location: (" << coneTranslation[0] << ", " << coneTranslation[1] << ", " << coneTranslation[2] << ")"
              << std::endl;
}


void refreshDisplay() {
    static long double timeMs = 0;

    static auto currentFrameTime = std::chrono::steady_clock::now();
    static auto lastFrameTime = currentFrameTime;

    currentFrameTime = std::chrono::steady_clock::now();

    glClearColor(0.1, 0.1, 0.1, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto elapsedDurationNs = duration_cast<std::chrono::nanoseconds>((currentFrameTime - lastFrameTime));

    long double elapsedMs = static_cast<long>(elapsedDurationNs.count()) / 1000000.0;

    timeMs += elapsedMs;

    auto timeS = static_cast<float>(timeMs / 1000);

    render(timeS, elapsedMs);

    lastFrameTime = currentFrameTime;

    glutSwapBuffers();

    std::this_thread::sleep_for(1ms);
}

void initCheckerboardTexture() {
    GLubyte texture_image[64][64][3];

    for (int i = 0; i < 64; i++) {
        for (int j = 0; j < 64; j++) {
            if ((i / 4 + j / 4) % 2 == 0) {
                texture_image[i][j][0] = 255;
                texture_image[i][j][1] = 255;
                texture_image[i][j][2] = 255;
            } else {
                texture_image[i][j][0] = 0;
                texture_image[i][j][1] = 0;
                texture_image[i][j][2] = 0;
            }
        }
    }

    glGenTextures(1, &checkerboardTexture);
    glBindTexture(GL_TEXTURE_2D, checkerboardTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 64, 64, 0, GL_RGB, GL_UNSIGNED_BYTE, texture_image);

    glBindTexture(GL_TEXTURE_2D, -1);
}

void windowResize(GLint newWidth, GLint newHeight) {
    float ratio = static_cast<float>(newHeight) / static_cast<float>(newWidth);

    glViewport(0, 0, newWidth, newHeight);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(80, 1 / ratio, 1, 100);
    gluLookAt(0, 0, 0, 0, 0, -1, 0, 1, 0);

    glMatrixMode(GL_MODELVIEW);


    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void onKeyDown(unsigned char character, int x, int y) {
    switch (character) {
        case 27:        // escape
            exit(0);
            break;
        case 'f':
            fogIndex = (fogIndex + 1) % 3;
            break;
    }
}

void onMenuActivated(int index) {
    switch (index) {
        case ITEM_INCREASE_SPEED:
            animationSpeed *= animationSpeedIncrementFactor;
            break;
        case ITEM_DECREASE_SPEED:
            animationSpeed /= animationSpeedIncrementFactor;
            break;
        case ITEM_TOGGLE_LIGHT_1:
        case ITEM_TOGGLE_LIGHT_2:
        case ITEM_TOGGLE_LIGHT_3:
        case ITEM_TOGGLE_LIGHT_4:
            enabledLights ^= index;
            break;
    }
}
