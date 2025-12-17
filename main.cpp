#include <GLUT/glut.h>
#include <cmath>
#include <iostream>

// Constants
const int WIDTH = 800;
const int HEIGHT = 600;
const int NUMBER = 10;

// Function prototypes
void convertMatrix(float m[4][4], float result[16]);
void vectorNormal(float v[3]);
float vectorDot(float a[3], float b[3]);

// Original Ball Position
float position[20][3] = {
    { 0.0f, 15.0f, 0.0f },
    { 5.0f, 20.0f, 5.0f },
    { -5.0f, 18.0f, -5.0f },
    { 8.0f, 22.0f, 2.0f },
    { -8.0f, 19.0f, -2.0f },
    { 3.0f, 25.0f, -8.0f },
    { -3.0f, 17.0f, 8.0f },
    { 10.0f, 23.0f, -5.0f },
    { -10.0f, 21.0f, 3.0f },
    { 0.0f, 28.0f, -3.0f },
    { -2.0f, 8.0f, 0.7f },
    { 10.0f, 9.5f, 1.1f },
    { 5.0f, 8.2f, 0.8f },
    { -3.5f, 7.8f, 0.9f },
    { 4.0f, 9.6f, 0.1f },
    { 6.0f, 10.8f, 0.2f },
    { -3.0f, 10.0f, 0.2f },
    { 5.0f, 13.0f, 0.6f },
    { 1.0f, 9.2f, 0.1f },
    { 2.0f, 8.6f, 0.6f }
};

// Original Ball Velocity
float velocity[20][3] = {
    { 0.5f, 0, 0.5f },
    { -0.8f, 0, -0.3f },
    { 0.6f, 0, 0.8f },
    { -1.2f, 0, 0.4f },
    { 0.9f, 0, -0.7f },
    { -0.4f, 0, 0.9f },
    { 1.1f, 0, -0.5f },
    { -0.7f, 0, -0.8f },
    { 0.3f, 0, 1.0f },
    { -1.0f, 0, -0.6f },
    { 0.6f, 0, 0.0f },
    { -1, 0, 0 },
    { -1, 0, 0 },
    { 0.3f, 0, 0 },
    { -1, 0, 0 },
    { 0.4f, 0, 0 },
    { -1, 0, 0 },
    { 0.6f, 0, 0 },
    { -1, 0, 0 },
    { 0.7f, 0, 0 }
};

// Ball class
class Ball {
public:
    float position[3];
    float velocity[3];
    float Matrix[4][4];

    Ball() {
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                Matrix[i][j] = (i == j) ? 1.0f : 0.0f;
    }

    Ball(float pos[3], float vel[3]) {
        for (int i = 0; i < 3; i++) {
            position[i] = pos[i];
            velocity[i] = vel[i];
        }

        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                Matrix[i][j] = (i == j) ? 1.0f : 0.0f;

        Matrix[0][3] = position[0];
        Matrix[1][3] = position[1];
        Matrix[2][3] = position[2];
    }

    void setVelocity(float vel[3]) {
        for (int i = 0; i < 3; i++)
            velocity[i] = vel[i];
    }

    void setPosition(float pos[3]) {
        for (int i = 0; i < 3; i++)
            position[i] = pos[i];

        Matrix[0][3] = position[0];
        Matrix[1][3] = position[1];
        Matrix[2][3] = position[2];
    }

    float distance(Ball& ball) {
        float dx = position[0] - ball.position[0];
        float dy = position[1] - ball.position[1];
        float dz = position[2] - ball.position[2];
        return sqrt(dx * dx + dy * dy + dz * dz);
    }

    bool collision(Ball& ball) {
        return distance(ball) < 1.0f;
    }

    void toOneDMatrix(float result[16]) {
        convertMatrix(Matrix, result);
    }
};

// Balls class
class Balls {
public:
    int number;
    Ball balls[NUMBER];
    float energyLoss;
    float timeIncrease;
    float acceleration[3];

    Balls(int num) {
        number = num;
        energyLoss = 0.8f;
        timeIncrease = 0.03f;
        acceleration[0] = 0;
        acceleration[1] = -2.0f;
        acceleration[2] = 0;

        for (int i = 0; i < number; i++) {
            balls[i] = Ball(position[i], velocity[i]);
        }
    }

    void ballCollision(int index) {
        Ball& current = balls[index];

        for (int i = index + 1; i < number; i++) {
            Ball& next = balls[i];

            if (current.collision(next)) {
                float x_axis[3];
                for (int j = 0; j < 3; j++) {
                    x_axis[j] = next.position[j] - current.position[j];
                }
                vectorNormal(x_axis);

                float d1[3];
                float velocityChange1[3];
                float u1 = vectorDot(x_axis, current.velocity);

                for (int j = 0; j < 3; j++) {
                    d1[j] = u1 * x_axis[j];
                    velocityChange1[j] = current.velocity[j] - d1[j];
                }

                for (int j = 0; j < 3; j++) {
                    x_axis[j] = current.position[j] - next.position[j];
                }
                vectorNormal(x_axis);

                float d2[3];
                float velocityChange2[3];
                float u2 = vectorDot(x_axis, next.velocity);

                for (int j = 0; j < 3; j++) {
                    d2[j] = u2 * x_axis[j];
                    velocityChange2[j] = next.velocity[j] - d2[j];
                }

                float newVelocity1[3];
                float newVelocity2[3];

                for (int j = 0; j < 3; j++) {
                    newVelocity1[j] = (d1[j] + d2[j] - (d1[j] - d2[j])) * 0.5f + velocityChange1[j];
                    newVelocity2[j] = (d1[j] + d2[j] - (d2[j] - d1[j])) * 0.5f + velocityChange2[j];
                }

                current.setVelocity(newVelocity1);
                next.setVelocity(newVelocity2);
            }
        }
    }

    void groundCollision(int index) {
        Ball& current = balls[index];
        if (current.position[1] < 0.5f) {
            current.velocity[1] = -energyLoss * current.velocity[1];
        }
    }

    void backWallCollision(int index) {
        Ball& current = balls[index];
        if (current.position[2] < -30.0f || current.position[2] > 30.0f) {
            current.velocity[2] = -energyLoss * current.velocity[2];
        }
    }

    void sideWallCollision(int index) {
        Ball& current = balls[index];
        if (current.position[0] < -30.0f || current.position[0] > 30.0f) {
            current.velocity[0] = -energyLoss * current.velocity[0];
        }
    }

    void ballAnimator(int index) {
        Ball& current = balls[index];

        groundCollision(index);
        backWallCollision(index);
        sideWallCollision(index);
        ballCollision(index);

        float newVelocity[3];
        float newPosition[3];

        for (int i = 0; i < 3; i++) {
            newVelocity[i] = current.velocity[i] + acceleration[i] * timeIncrease;
            newPosition[i] = current.position[i] + current.velocity[i] * timeIncrease;
        }

        current.setVelocity(newVelocity);
        current.setPosition(newPosition);
    }
};

// Global variables
Balls balls(NUMBER);

// Utility functions
void convertMatrix(float m[4][4], float result[16]) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result[i + j * 4] = m[i][j];
        }
    }
}

void vectorNormal(float v[3]) {
    float magnitude = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    if (magnitude != 0) {
        v[0] /= magnitude;
        v[1] /= magnitude;
        v[2] /= magnitude;
    }
}

float vectorDot(float a[3], float b[3]) {
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

void enable() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
}

void display() {
    enable();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(30.0f, 30.0f, -20.0f,
        0.0, 0.0, 0.0,
        0.0, 1.0, 0.0);

    // Render the ground
    glBegin(GL_LINES);
    glColor3f(1.0f, 0.0f, 0.0f);
    for (float x = -30; x <= 30; x += 5.0f) {
        glVertex3f(x, 0, -30);
        glVertex3f(x, 0, 30);
    }
    for (float z = -30; z <= 30; z += 5.0f) {
        glVertex3f(-30, 0, z);
        glVertex3f(30, 0, z);
    }

    // Render the back wall
    glColor3f(0.0f, 1.0f, 0.0f);
    for (float y = 0; y <= 100; y += 0.25f) {
        glVertex3f(-30, y, -30);
        glVertex3f(30, y, -30);
    }
    for (float x = -30; x <= 30; x += 5.0f) {
        glVertex3f(x, 0, -30);
        glVertex3f(x, 150, -30);
    }

    // Render the front wall
    glColor3f(1.0f, 1.0f, 1.0f);
    for (float y = 0; y <= 100; y += 0.25f) {
        glVertex3f(-30, y, 30);
        glVertex3f(30, y, 30);
    }
    for (float x = -30; x <= 30; x += 5.0f) {
        glVertex3f(x, 0, 30);
        glVertex3f(x, 150, 30);
    }

    // Render the left wall
    glColor3f(0.0f, 0.0f, 1.0f);
    for (float y = 0; y <= 100; y += 0.25f) {
        glVertex3f(-30, y, -30);
        glVertex3f(-30, y, 30);
    }
    for (float z = -30; z <= 30; z += 5.0f) {
        glVertex3f(-30, 0, z);
        glVertex3f(-30, 100, z);
    }

    // Render the right wall
    for (float y = 0; y <= 100; y += 0.25f) {
        glVertex3f(30, y, -30);
        glVertex3f(30, y, 30);
    }
    for (float z = -30; z <= 30; z += 5.0f) {
        glVertex3f(30, 0, z);
        glVertex3f(30, 100, z);
    }
    glEnd();

    // Enable lighting for balls
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    float LightAmbient[] = { 0.2f, 0.2f, 0.5f, 1.0f };
    float LightDiffuse[] = { 0.6f, 0.6f, 0.8f, 1.0f };
    float LightSpecular[] = { 0.8f, 0.8f, 1.0f, 1.0f };
    float LightPosition[] = { 5.0f, 5.0f, 5.0f, 1.0f };

    glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, LightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, LightSpecular);
    glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);

    float material_Ka[] = { 0.2f, 0.6f, 1.0f, 1.0f };
    float material_Kd[] = { 0.3f, 0.7f, 0.9f, 1.0f };
    float material_Ks[] = { 0.8f, 0.9f, 1.0f, 1.0f };
    float material_Ke[] = { 0.0f, 0.1f, 0.2f, 1.0f };
    float material_Se = 10.0f;

    glMaterialfv(GL_FRONT, GL_AMBIENT, material_Ka);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, material_Kd);
    glMaterialfv(GL_FRONT, GL_SPECULAR, material_Ks);
    glMaterialfv(GL_FRONT, GL_EMISSION, material_Ke);
    glMaterialf(GL_FRONT, GL_SHININESS, material_Se);

    // Render the balls
    float matrix[16];
    for (int i = 0; i < balls.number; i++) {
        glPushMatrix();
        balls.ballAnimator(i);
        balls.balls[i].toOneDMatrix(matrix);
        glMultMatrixf(matrix);
        glutSolidSphere(0.5, 20, 20);
        glPopMatrix();
    }

    glutSwapBuffers();
}

void timer(int value) {
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

void reshape(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(65.0, (float)width / (float)height, 1.0, 100.0);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Lab3 Physics-Based Motion Control System");

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(0, timer, 0);

    glutMainLoop();
    return 0;
}
