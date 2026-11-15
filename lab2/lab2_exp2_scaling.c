/*
 * Lab 2 - Experiment 2: 3D Scaling Transformation (OpenGL)
 *
 * A cube with corners at (+-1, +-1, +-1) is scaled using a 4x4 scaling
 * matrix S. Three cases are shown:
 *   1. Uniform scaling about the origin      (sx = sy = sz = 1.5)
 *   2. Non-uniform scaling about the origin  (sx = 2, sy = 1, sz = 0.5)
 *   3. Scaling about a pivot point (1,1,1)   M = T^-1 x S x T
 *
 * Keys: 1, 2, 3 switch case    q / Esc quits
 *
 * Build (macOS):  clang lab2_exp2_scaling.c -o scaling -framework OpenGL -framework GLUT
 * Build (Linux):  gcc lab2_exp2_scaling.c -o scaling -lGL -lGLU -lglut -lm
 * Run:            ./scaling
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

typedef struct { float x, y, z; } Point3;

Point3 cube[8];
Point3 scaled[3][8];          /* result for each of the 3 cases */
int current = 0;              /* which case is on screen        */

int edges[12][2] = {
    {0,1},{2,3},{4,5},{6,7},
    {0,2},{1,3},{4,6},{5,7},
    {0,4},{1,5},{2,6},{3,7}
};

const char *caseName[3] = {
    "Case 1: uniform scaling about origin (1.5, 1.5, 1.5)",
    "Case 2: non-uniform scaling about origin (2, 1, 0.5)",
    "Case 3: uniform scaling 0.5 about pivot (1, 1, 1)"
};

/* ---------- matrix helpers ---------- */
void identity(float M[4][4])
{
    for (int r = 0; r < 4; r++)
        for (int c = 0; c < 4; c++)
            M[r][c] = (r == c) ? 1.0f : 0.0f;
}

void makeScaling(float M[4][4], float sx, float sy, float sz)
{
    identity(M);
    M[0][0] = sx; M[1][1] = sy; M[2][2] = sz;
}

void makeTranslation(float M[4][4], float tx, float ty, float tz)
{
    identity(M);
    M[0][3] = tx; M[1][3] = ty; M[2][3] = tz;
}

/* R = A x B */
void multiply(float A[4][4], float B[4][4], float R[4][4])
{
    float tmp[4][4];
    for (int r = 0; r < 4; r++)
        for (int c = 0; c < 4; c++) {
            tmp[r][c] = 0;
            for (int k = 0; k < 4; k++)
                tmp[r][c] += A[r][k] * B[k][c];
        }
    for (int r = 0; r < 4; r++)
        for (int c = 0; c < 4; c++)
            R[r][c] = tmp[r][c];
}

Point3 transform(float M[4][4], Point3 p)
{
    float in[4]  = {p.x, p.y, p.z, 1.0f};
    float out[4] = {0, 0, 0, 0};
    for (int r = 0; r < 4; r++)
        for (int c = 0; c < 4; c++)
            out[r] += M[r][c] * in[c];
    Point3 q = {out[0] / out[3], out[1] / out[3], out[2] / out[3]};
    return q;
}

void printMatrix(const char *name, float M[4][4])
{
    printf("%s =\n", name);
    for (int r = 0; r < 4; r++)
        printf("  | %6.2f %6.2f %6.2f %6.2f |\n", M[r][0], M[r][1], M[r][2], M[r][3]);
}

void printTable(int k)
{
    printf("\n Vertex |   Original P (x, y, z)   |   Scaled P' (x', y', z')\n");
    printf("--------+--------------------------+------------------------------\n");
    for (int i = 0; i < 8; i++)
        printf("   %d    | (%5.1f, %5.1f, %5.1f)    |  (%5.2f, %5.2f, %5.2f)\n", i,
               cube[i].x, cube[i].y, cube[i].z,
               scaled[k][i].x, scaled[k][i].y, scaled[k][i].z);
}

/* ---------- drawing ---------- */
void drawCube(Point3 v[8])
{
    glBegin(GL_LINES);
    for (int e = 0; e < 12; e++) {
        Point3 a = v[edges[e][0]], b = v[edges[e][1]];
        glVertex3f(a.x, a.y, a.z);
        glVertex3f(b.x, b.y, b.z);
    }
    glEnd();
}

void drawAxes(void)
{
    glLineWidth(1.0f);
    glBegin(GL_LINES);
    glColor3f(0.6f, 0.2f, 0.2f); glVertex3f(-4, 0, 0); glVertex3f(4, 0, 0);
    glColor3f(0.2f, 0.6f, 0.2f); glVertex3f(0, -4, 0); glVertex3f(0, 4, 0);
    glColor3f(0.2f, 0.2f, 0.6f); glVertex3f(0, 0, -4); glVertex3f(0, 0, 4);
    glEnd();
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    drawAxes();

    glLineWidth(2.0f);
    glColor3f(1.0f, 1.0f, 1.0f);              /* original: white */
    drawCube(cube);

    glColor3f(0.0f, 1.0f, 0.4f);              /* scaled: green */
    drawCube(scaled[current]);

    if (current == 2) {                       /* mark the pivot point */
        glPointSize(10.0f);
        glColor3f(1.0f, 0.2f, 0.2f);
        glBegin(GL_POINTS);
        glVertex3f(1, 1, 1);
        glEnd();
    }
    glutSwapBuffers();
}

void reshape(int w, int h)
{
    if (h == 0) h = 1;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)w / h, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(6, 5, 8,   0, 0, 0,   0, 1, 0);
}

void keyboard(unsigned char key, int x, int y)
{
    (void)x; (void)y;
    if (key >= '1' && key <= '3') {
        current = key - '1';
        glutSetWindowTitle(caseName[current]);
        printf("\n>>> Showing %s\n", caseName[current]);
        glutPostRedisplay();
    } else if (key == 'q' || key == 27) {
        exit(0);
    }
}

int main(int argc, char **argv)
{
    for (int i = 0; i < 8; i++) {
        cube[i].x = (i & 1) ? 1.0f : -1.0f;
        cube[i].y = (i & 2) ? 1.0f : -1.0f;
        cube[i].z = (i & 4) ? 1.0f : -1.0f;
    }

    float S1[4][4], S2[4][4], S3[4][4], T[4][4], Tinv[4][4], M[4][4];

    /* Case 1: uniform, about origin */
    makeScaling(S1, 1.5f, 1.5f, 1.5f);
    /* Case 2: non-uniform, about origin */
    makeScaling(S2, 2.0f, 1.0f, 0.5f);
    /* Case 3: about pivot (px,py,pz): M = T^-1 x S x T */
    float px = 1, py = 1, pz = 1;
    makeTranslation(T, -px, -py, -pz);      /* move pivot to origin */
    makeScaling(S3, 0.5f, 0.5f, 0.5f);      /* scale                */
    makeTranslation(Tinv, px, py, pz);      /* move back            */
    multiply(S3, T, M);
    multiply(Tinv, M, M);

    for (int i = 0; i < 8; i++) {
        scaled[0][i] = transform(S1, cube[i]);
        scaled[1][i] = transform(S2, cube[i]);
        scaled[2][i] = transform(M, cube[i]);
    }

    printf("===== LAB 2 - EXPERIMENT 2: 3D SCALING =====\n");

    printf("\n--- %s ---\n", caseName[0]);
    printMatrix("S", S1);
    printTable(0);

    printf("\n--- %s ---\n", caseName[1]);
    printMatrix("S", S2);
    printTable(1);

    printf("\n--- %s ---\n", caseName[2]);
    printMatrix("T (to origin)", T);
    printMatrix("S", S3);
    printMatrix("T^-1 (back)", Tinv);
    printMatrix("M = T^-1 x S x T", M);
    printTable(2);
    printf("\nNote: vertex 7 = (1,1,1) is the pivot, so it does not move in case 3.\n");

    printf("\nWindow: WHITE = original, GREEN = scaled, RED dot = pivot.\n");
    printf("Press 1, 2, 3 to switch case, q to quit.\n");

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow(caseName[0]);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}
