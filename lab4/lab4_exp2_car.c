/*
 * Lab 4 - Experiment 2: 2D Car using OpenGL primitives
 *
 * Uses only glBegin/glEnd, glVertex2f and glColor3f.
 * Coordinates follow the same y-down convention as the house
 * (gluOrtho2D(0, 600, 400, 0)).
 *
 * Correction to the brief: with the body at y = 150..200, the brief puts the
 * wheel centres at y = 150 (the TOP of the body). They are moved to y = 200
 * (the bottom) so the car matches the expected output. The cabin is drawn as
 * a trapezoid, as in the expected output picture.
 *
 * Build (macOS):  clang lab4_exp2_car.c -o car -framework OpenGL -framework GLUT
 * Build (Linux):  gcc lab4_exp2_car.c -o car -lGL -lGLU -lglut -lm
 * Run:            ./car
 */
#define GL_SILENCE_DEPRECATION
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <stdio.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/* ---------- helper primitives ---------- */
void drawRectangle(float x1, float y1, float x2, float y2)
{
    glBegin(GL_LINE_LOOP);
    glVertex2f(x1, y1);
    glVertex2f(x2, y1);
    glVertex2f(x2, y2);
    glVertex2f(x1, y2);
    glEnd();
}

void drawCircle(float cx, float cy, float r)
{
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 100; i++) {
        float a = 2.0f * (float)M_PI * i / 100.0f;
        glVertex2f(cx + r * cosf(a), cy + r * sinf(a));
    }
    glEnd();
}

void drawPoly(int n, float pts[])
{
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < n; i++)
        glVertex2f(pts[2 * i], pts[2 * i + 1]);
    glEnd();
}

/* ---------- the car ---------- */
void drawCar(void)
{
    /* Body */
    glColor3f(0.0f, 0.5f, 1.0f);                  /* blue */
    drawRectangle(200, 150, 380, 200);            /* main chassis */

    /* Cabin (trapezoid) */
    glColor3f(0.9f, 0.9f, 0.9f);
    float cabin[] = {225, 150, 255, 115, 325, 115, 355, 150};
    drawPoly(4, cabin);

    /* Wheels */
    glColor3f(0.8f, 0.8f, 0.8f);
    drawCircle(240, 200, 20);                     /* left wheel  */
    drawCircle(340, 200, 20);                     /* right wheel */

    /* Wheel hubs */
    glColor3f(0.6f, 0.6f, 0.6f);
    drawCircle(240, 200, 8);
    drawCircle(340, 200, 8);
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    drawCar();
    glFlush();

    printf("\n========== LAB REPORT: EXPERIMENT 2 - CAR ==========\n");
    printf(" Body   : rectangle (200,150)-(380,200)          GL_LINE_LOOP, 4 vertices\n");
    printf(" Cabin  : trapezoid (225,150),(255,115),(325,115),(355,150)  4 vertices\n");
    printf(" Wheels : circles at (240,200),(340,200) r=20    GL_LINE_LOOP, 100 vertices each\n");
    printf(" Hubs   : circles at (240,200),(340,200) r=8     GL_LINE_LOOP, 100 vertices each\n");
    printf(" Projection: gluOrtho2D(0, 600, 400, 0)  (y grows downwards)\n");
    printf("===================================================\n");
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(600, 400);
    glutCreateWindow("Lab 4 - Exp 2: Car");
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glLineWidth(2.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 600, 400, 0);
    glMatrixMode(GL_MODELVIEW);
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
