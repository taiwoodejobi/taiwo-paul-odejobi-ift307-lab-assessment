"""
Lab 3 - Experiment 3: Orthographic vs Perspective Projection
Build both projection matrices, project the same cube, and compare
the length of the edge x = -1 .. +1 on the faces at z = 4 and z = 6.
"""
import os
import numpy as np
import matplotlib.pyplot as plt

VERTS = [(x, y, z) for x in (-1, 1) for y in (-1, 1) for z in (-1, 1)]
EDGES = [(i, j) for i in range(8) for j in range(i + 1, 8)
         if sum(a != b for a, b in zip(VERTS[i], VERTS[j])) == 1]
CUBE = [(x, y, z + 5) for x, y, z in VERTS]          # centred at z = 5

# ---- 1. Projection matrices ----
l, r, b, t, n, f = -2, 2, -2, 2, 1, 10
P_ortho = np.array([
    [2 / (r - l), 0,           0,            -(r + l) / (r - l)],
    [0,           2 / (t - b), 0,            -(t + b) / (t - b)],
    [0,           0,           -2 / (f - n), -(f + n) / (f - n)],
    [0,           0,           0,            1],
])

focal = 1
P_persp = np.array([            # projects onto the plane z = 1, w' = z
    [focal, 0,     0, 0],
    [0,     focal, 0, 0],
    [0,     0,     1, 0],
    [0,     0,     1, 0],
])

np.set_printoptions(precision=3, suppress=True)
print("Orthographic matrix:\n", P_ortho)
print("\nPerspective matrix (f = 1):\n", P_persp)


def project(M, p):
    v = M @ np.array([*p, 1.0])      # homogeneous coordinates
    return v[:2] / v[3]              # homogeneous divide (w = 1 for ortho)


# ---- 2. Edge length vs depth ----
print("\nEdge from x = -1 to x = +1 (y = 1):")
print(f"{'depth z':>8} | {'ortho length':>12} | {'perspective length':>18}")
lengths = {}
for z in (4, 6):
    o = project(P_ortho, (1, 1, z))[0] - project(P_ortho, (-1, 1, z))[0]
    p = project(P_persp, (1, 1, z))[0] - project(P_persp, (-1, 1, z))[0]
    lengths[z] = (o, p)
    print(f"{z:>8} | {o:>12.3f} | {p:>18.3f}")

print("\nPerspective formula: L = x2' - x1' = 1/z - (-1/z) = 2/z")
print(f"Ratio near/far (perspective) = {lengths[4][1]:.3f} / {lengths[6][1]:.3f}"
      f" = {lengths[4][1] / lengths[6][1]:.1f}")
print(f"Ratio near/far (orthographic) = {lengths[4][0] / lengths[6][0]:.1f}  (no depth effect)")
print("Conclusion: the nearer face appears 1.5x larger under perspective projection.")

# ---- 3. Visualise both ----
fig, axes = plt.subplots(1, 2, figsize=(13, 6))
for ax, (name, M) in zip(axes, [("Orthographic", P_ortho), ("Perspective", P_persp)]):
    pts = [project(M, v) for v in CUBE]
    for i, j in EDGES:
        z1, z2 = CUBE[i][2], CUBE[j][2]
        style = "b-" if z1 == z2 == 4 else ("r--" if z1 == z2 == 6 else "g-")
        ax.plot([pts[i][0], pts[j][0]], [pts[i][1], pts[j][1]], style, lw=2)
    ax.plot([], [], "b-", label="face at z = 4")
    ax.plot([], [], "r--", label="face at z = 6")
    ax.set_aspect("equal")
    ax.grid(True)
    ax.set_title(name)
    ax.legend(loc="lower right", fontsize=8)

plt.suptitle("Same cube (centre z = 5): orthographic keeps size, perspective shrinks with depth")
plt.tight_layout()
out_dir = os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "screenshots")
os.makedirs(out_dir, exist_ok=True)
plt.savefig(os.path.join(out_dir, "lab3_exp3_comparison.png"), dpi=120)
plt.show()
