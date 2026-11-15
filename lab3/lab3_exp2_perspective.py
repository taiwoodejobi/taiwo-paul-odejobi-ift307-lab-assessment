"""
Lab 3 - Experiment 2: Perspective (Pinhole) Projection of a Cube
x' = f*x / z,  y' = f*y / z   ->  farther objects appear smaller.
"""
import os
import matplotlib.pyplot as plt

VERTS = [(x, y, z) for x in (-1, 1) for y in (-1, 1) for z in (-1, 1)]
EDGES = [(i, j) for i in range(8) for j in range(i + 1, 8)
         if sum(a != b for a, b in zip(VERTS[i], VERTS[j])) == 1]

# Translate the cube so its centre is at z = 5 (faces at z = 4 and z = 6)
CUBE = [(x, y, z + 5) for x, y, z in VERTS]

W, H = 800, 600
VIEW = 0.6                        # view window [-0.6, 0.6] in projected units


def perspective(p, f=1.0):
    x, y, z = p
    return f * x / z, f * y / z   # perspective divide


def to_pixel(x, y):
    xn = (x + VIEW) / (2 * VIEW)
    yn = (y + VIEW) / (2 * VIEW)
    return xn * W, (1 - yn) * H


# ---- Question 1 ----
near = perspective((1, 1, 4))
far = perspective((1, 1, 6))
print("Q1: projected coordinates (f = 1)")
print(f"    (1, 1, 4) -> ({near[0]:.3f}, {near[1]:.3f})   <- nearer, LARGER")
print(f"    (1, 1, 6) -> ({far[0]:.3f}, {far[1]:.3f})")

# ---- Question 2 ----
print("\nQ2: effect of doubling f")
for f in (1, 2):
    p = perspective((1, 1, 4), f)
    print(f"    f = {f}: (1, 1, 4) -> ({p[0]:.3f}, {p[1]:.3f})")
print("    -> Doubling f doubles every projected coordinate (image zooms in).")

# ---- Draw: f = 1 and f = 2 side by side ----
fig, axes = plt.subplots(1, 2, figsize=(14, 5.5))
for ax, f in zip(axes, (1, 2)):
    pix = [to_pixel(*perspective(v, f)) for v in CUBE]
    for i, j in EDGES:
        front = CUBE[i][2] == 4 and CUBE[j][2] == 4
        back = CUBE[i][2] == 6 and CUBE[j][2] == 6
        style = "b-" if front else ("r--" if back else "g-")
        ax.plot([pix[i][0], pix[j][0]], [pix[i][1], pix[j][1]], style, lw=2)
    ax.plot([], [], "b-", label="front face z=4 (bigger)")
    ax.plot([], [], "r--", label="back face z=6 (smaller)")
    ax.plot([], [], "g-", label="depth edges (converge)")
    ax.set_xlim(0, W)
    ax.set_ylim(H, 0)
    ax.set_title(f"Perspective projection, f = {f}")
    ax.set_xlabel("pixel x")
    ax.set_ylabel("pixel y")
    ax.grid(True)
    ax.legend(loc="center", fontsize=8)

plt.tight_layout()
out_dir = os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "screenshots")
os.makedirs(out_dir, exist_ok=True)
plt.savefig(os.path.join(out_dir, "lab3_exp2_perspective.png"), dpi=120)
plt.show()
