/*
 * Lab 2 - Experiment 1: 3D Translation Transformation (OpenGL)
 *
 * A cube with corners at (+-1, +-1, +-1) is translated by (tx, ty, tz)
 * using a 4x4 homogeneous translation matrix: P' = T x P.
 * The maths is done by hand (no glTranslatef) so every step is visible.
 *
 * Build (macOS):  clang lab2_exp1_translation.c -o translation -framework OpenGL -framework GLUT
 * Build (Linux):  gcc lab2_exp1_translation.c -o translation -lGL -lGLU -lglut -lm
 * Run:            ./translation
 */
#define GL_SILENCE_DEPRECATION
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <stdio.h>
#include <math.h>

typedef struct { float x, y, z; } Point3;

/* Cube corners: bit 0 -> x, bit 1 -> y, bit 2 -> z */
Point3 cube[8];
Point3 moved[8];

/* The 12 edges of a cube (pairs of corner indices) */
int edges[12][2] = {
    {0,1},{2,3},{4,5},{6,7},   /* along x */
    {0,2},{1,3},{4,6},{5,7},   /* along y */
    {0,4},{1,5},{2,6},{3,7}    /* along z */
};

float tx = 3.0f, ty = 2.0f, tz = -1.0f;   /* translation distances */
float T[4][4];                            /* translation matrix    */

/* Step 3: build the 4x4 translation matrix */
void makeTranslation(float M[4][4], float tx, float ty, float tz)
{
    float I[4][4] = {
        {1, 0, 0, tx},
        {0, 1, 0, ty},
        {0, 0, 1, tz},
        {0, 0, 0, 1 }
    };
    for (int r = 0; r < 4; r++)
        for (int c = 0; c < 4; c++)
            M[r][c] = I[r][c];
}

/* Step 4: P' = M x P, with P = [x y z 1] (homogeneous coordinates) */
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
    glColor3f(0.6f, 0.2f, 0.2f); glVertex3f(-6, 0, 0); glVertex3f(6, 0, 0);  /* x */
    glColor3f(0.2f, 0.6f, 0.2f); glVertex3f(0, -6, 0); glVertex3f(0, 6, 0);  /* y */
    glColor3f(0.2f, 0.2f, 0.6f); glVertex3f(0, 0, -6); glVertex3f(0, 0, 6);  /* z */
    glEnd();
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    drawAxes();

    glLineWidth(2.0f);
    glColor3f(1.0f, 1.0f, 1.0f);      /* original cube: white */
    drawCube(cube);

    glColor3f(1.0f, 0.8f, 0.0f);      /* translated cube: yellow */
    drawCube(moved);

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
    gluLookAt(8, 7, 12,   1.5, 1, -0.5,   0, 1, 0);   /* camera position, target, up */
}

int main(int argc, char **argv)
{
    /* Build the cube */
    for (int i = 0; i < 8; i++) {
        cube[i].x = (i & 1) ? 1.0f : -1.0f;
        cube[i].y = (i & 2) ? 1.0f : -1.0f;
        cube[i].z = (i & 4) ? 1.0f : -1.0f;
    }

    /* Build T and apply it to every vertex (step 6) */
    makeTranslation(T, tx, ty, tz);
    for (int i = 0; i < 8; i++)
        moved[i] = transform(T, cube[i]);

    /* Console report */
    printf("===== LAB 2 - EXPERIMENT 1: 3D TRANSLATION =====\n");
    printf("Translation distances: tx = %.1f, ty = %.1f, tz = %.1f\n\n", tx, ty, tz);
    printMatrix("T", T);
    printf("\n Vertex |   Original P (x, y, z)   |  Translated P' (x', y', z')\n");
    printf("--------+--------------------------+------------------------------\n");
    for (int i = 0; i < 8; i++)
        printf("   %d    | (%5.1f, %5.1f, %5.1f)    |  (%5.1f, %5.1f, %5.1f)\n", i,
               cube[i].x, cube[i].y, cube[i].z, moved[i].x, moved[i].y, moved[i].z);
    printf("\nCheck: x' = x + tx, y' = y + ty, z' = z + tz (shape and size unchanged).\n");
    printf("Window: WHITE = original cube, YELLOW = translated cube.\n");

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Lab 2 - Exp 1: 3D Translation");
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMainLoop();
    return 0;
}
