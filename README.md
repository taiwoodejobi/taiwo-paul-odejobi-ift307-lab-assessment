# MIVA-IFT 307: Computer Graphics and Animation Lab Assessment

| Lab | Topic | Language |
|---|---|---|
| Lab 1 | DDA line, Bresenham line, Midpoint circle | Python + matplotlib |
| Lab 2 | 3D translation and scaling (4×4 matrices) | C + OpenGL/GLUT |
| Lab 3 | Orthographic vs perspective projection | Python + matplotlib + NumPy |
| Lab 4 | 2D house and car | C + OpenGL/GLUT |

## Python labs (1 and 3)

Install the dependencies once:
```bash
pip install -r requirements.txt
```
Run any experiment, for example:
```bash
python lab1/lab1_exp1_dda.py
```
Each script prints its results and opens a plot. A copy of the plot is saved to `screenshots/`.

## C / OpenGL labs (2 and 4)

**macOS** (OpenGL and GLUT are built in):
```bash
clang lab2/lab2_exp1_translation.c -o translation -framework OpenGL -framework GLUT
./translation
```
**Linux** (needs `freeglut3-dev`):
```bash
gcc lab2/lab2_exp1_translation.c -o translation -lGL -lGLU -lglut -lm
./translation
```
The same commands work for every `.c` file. Each file's header comment shows its exact build line.

Keyboard controls:
- `lab2_exp2_scaling`: **1 / 2 / 3** switch scaling case, **q** quits
- `lab4_combined`: **h** house, **c** car, **q** quits

## Results summary

| Experiment | Result |
|---|---|
| Lab 1 Exp 1, DDA (2,3)→(10,8) | 9 pixels, float ops = 2 + 2×steps = **18** |
| Lab 1 Exp 2, Bresenham | Same 9 pixels, **0** float ops (integer decision variable p) |
| Lab 1 Exp 3, Midpoint circle r=10 | **8** pixels computed in first octant → 56 total via 8-way symmetry |
| Lab 2 Exp 1, Translation (3, 2, −1) | x' = x+tx, y' = y+ty, z' = z+tz |
| Lab 2 Exp 2, Scaling | origin (uniform and non-uniform) and pivot M = T⁻¹·S·T |
| Lab 3 Exp 1, Orthographic | (1,1,1) → pixel **(600, 150)**; size unchanged with z |
| Lab 3 Exp 2, Perspective | (1,1,4) → **(0.25, 0.25)**; doubling f doubles size |
| Lab 3 Exp 3, Comparison | L = 2/z → 0.5 vs 0.333, ratio **1.5** |
| Lab 4, House and car | OpenGL line primitives, keyboard switching, console lab report |

## Notes on the brief
- **DDA:** the brief's pseudocode uses `abs(dx)`, which draws right-to-left lines in the wrong direction. This version keeps the sign of dx/dy.
- **Lab 4 coordinates:** the brief's shapes use y growing downwards, so `gluOrtho2D(0, 600, 400, 0)` is used. Otherwise the house is upside down.
- **Lab 4 car:** the brief places the wheels at the top of the body (y = 150). They are moved to y = 200 (the bottom) to match the expected output.
