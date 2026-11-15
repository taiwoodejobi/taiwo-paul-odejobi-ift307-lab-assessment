/*
 * Lab 4 - Combined program: House and Car with keyboard view switching
 *
 * Meets the brief's requirements:
 *   - OpenGL vertex-based rendering (glBegin/glEnd, glVertex2f, glColor3f)
 *   - Modular rendering via separate functions (drawHouse, drawCar)
 *   - Interactive view switching with the keyboard
 *   - A structured lab report printed to the console each time a shape is displayed
 *
 * Keys:  h = house   c = car   q / Esc = quit
 *
 * Build (macOS):  clang lab4_combined.c -o shapes -framework OpenGL -framework GLUT
 * Build (Linux):  gcc lab4_combined.c -o shapes -lGL -lGLU -lglut -lm
 * Run:            ./shapes
 */
#define GL_SILENCE_DEPRECATION
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

enum { VIEW_HOUSE, VIEW_CAR };
int view = VIEW_HOUSE;
int reportCount = 0;

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

/* ---------- shapes (modular rendering) ---------- */
void drawHouse(void)
{
    glColor3f(0.7f, 0.4f, 0.2f);                  /* base */
    drawRectangle(60, 80, 150, 200);
    glColor3f(0.5f, 0.2f, 0.0f);                  /* door */
    drawRectangle(95, 140, 120, 200);
    glColor3f(0.8f, 0.1f, 0.1f);                  /* roof */
    float roof[] = {60, 80, 100, 15, 150, 80};
    drawPoly(3, roof);
    glColor3f(0.4f, 0.4f, 0.4f);                  /* chimney */
    drawCircle(100, 60, 10);
}

void drawCar(void)
{
    glColor3f(0.0f, 0.5f, 1.0f);                  /* body */
    drawRectangle(200, 150, 380, 200);
    glColor3f(0.9f, 0.9f, 0.9f);                  /* cabin */
    float cabin[] = {225, 150, 255, 115, 325, 115, 355, 150};
    drawPoly(4, cabin);
    glColor3f(0.8f, 0.8f, 0.8f);                  /* wheels */
    drawCircle(240, 200, 20);
    drawCircle(340, 200, 20);
    glColor3f(0.6f, 0.6f, 0.6f);                  /* hubs */
    drawCircle(240, 200, 8);
    drawCircle(340, 200, 8);
}

/* ---------- console lab reports ---------- */
void reportHouse(void)
{
    printf("\n========== LAB REPORT #%d: EXPERIMENT 1 - HOUSE ==========\n", ++reportCount);
    printf(" Function   : drawHouse()\n");
    printf(" Primitives : 2 rectangles, 1 triangle, 1 circle\n");
    printf(" Vertices   : 4 + 4 + 3 + 100 = 111 (all GL_LINE_LOOP)\n");
    printf(" Colours    : base brown, door dark brown, roof red, chimney grey\n");
    printf("=========================================================\n");
}

void reportCar(void)
{
    printf("\n========== LAB REPORT #%d: EXPERIMENT 2 - CAR ==========\n", ++reportCount);
    printf(" Function   : drawCar()\n");
    printf(" Primitives : 1 rectangle, 1 trapezoid, 4 circles\n");
    printf(" Vertices   : 4 + 4 + 4 x 100 = 408 (all GL_LINE_LOOP)\n");
    printf(" Colours    : body blue, cabin white, wheels light grey, hubs grey\n");
    printf("=======================================================\n");
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    if (view == VIEW_HOUSE) {
        drawHouse();
        reportHouse();
    } else {
        drawCar();
        reportCar();
    }
    glFlush();
}

void keyboard(unsigned char key, int x, int y)
{
    (void)x; (void)y;
    switch (key) {
    case 'h': case 'H':
        view = VIEW_HOUSE;
        glutSetWindowTitle("Lab 4 - House  (h = house, c = car, q = quit)");
        glutPostRedisplay();
        break;
    case 'c': case 'C':
        view = VIEW_CAR;
        glutSetWindowTitle("Lab 4 - Car  (h = house, c = car, q = quit)");
        glutPostRedisplay();
        break;
    case 'q': case 'Q': case 27:
        printf("\nExiting. %d report(s) printed.\n", reportCount);
        exit(0);
    }
}

int main(int argc, char **argv)
{
    printf("Lab 4 - 2D objects with OpenGL\n");
    printf("Controls: h = house, c = car, q = quit\n");

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(600, 400);
    glutCreateWindow("Lab 4 - House  (h = house, c = car, q = quit)");
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glLineWidth(2.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 600, 400, 0);
    glMatrixMode(GL_MODELVIEW);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}
