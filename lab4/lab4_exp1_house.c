/*
 * Lab 4 - Experiment 1: 2D House using OpenGL primitives
 *
 * Uses only glBegin/glEnd, glVertex2f and glColor3f.
 * NOTE: the brief's coordinates treat y as growing DOWNWARDS (roof at y=15,
 * base at y=80..200), so the projection is gluOrtho2D(0, 600, 400, 0).
 * With gluOrtho2D(0, 600, 0, 400) the house would appear upside down.
 *
 * Build (macOS):  clang lab4_exp1_house.c -o house -framework OpenGL -framework GLUT
 * Build (Linux):  gcc lab4_exp1_house.c -o house -lGL -lGLU -lglut -lm
 * Run:            ./house
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
        float a = 2.0f * (float)M_PI * i / 100.0f;   /* trigonometric sampling */
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

/* ---------- the house (from the lab brief) ---------- */
void drawHouse(void)
{
    /* Base */
    glColor3f(0.7f, 0.4f, 0.2f);          /* brown */
    drawRectangle(60, 80, 150, 200);

    /* Door */
    glColor3f(0.5f, 0.2f, 0.0f);
    drawRectangle(95, 140, 120, 200);

    /* Roof */
    glColor3f(0.8f, 0.1f, 0.1f);          /* red */
    float roof[] = {60, 80, 100, 15, 150, 80};
    drawPoly(3, roof);

    /* Chimney */
    glColor3f(0.4f, 0.4f, 0.4f);
    drawCircle(100, 60, 10);
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    drawHouse();
    glFlush();

    printf("\n========== LAB REPORT: EXPERIMENT 1 - HOUSE ==========\n");
    printf(" Base    : rectangle (60,80)-(150,200)   GL_LINE_LOOP, 4 vertices\n");
    printf(" Door    : rectangle (95,140)-(120,200)  GL_LINE_LOOP, 4 vertices\n");
    printf(" Roof    : triangle (60,80),(100,15),(150,80)  GL_LINE_LOOP, 3 vertices\n");
    printf(" Chimney : circle centre (100,60) r=10   GL_LINE_LOOP, 100 vertices\n");
    printf(" Projection: gluOrtho2D(0, 600, 400, 0)  (y grows downwards)\n");
    printf("=====================================================\n");
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(600, 400);
    glutCreateWindow("Lab 4 - Exp 1: House");
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);   /* black background */
    glLineWidth(2.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 600, 400, 0);
    glMatrixMode(GL_MODELVIEW);
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
