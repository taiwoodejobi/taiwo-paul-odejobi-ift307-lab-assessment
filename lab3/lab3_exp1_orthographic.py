"""
Lab 3 - Experiment 1: Orthographic (Parallel) Projection of a Cube
(x, y, z) -> (x, y): depth is simply dropped, so size never changes with z.
"""
import os
import matplotlib.pyplot as plt

# Cube with corners at (+-1, +-1, +-1)
VERTS = [(x, y, z) for x in (-1, 1) for y in (-1, 1) for z in (-1, 1)]
# An edge joins two corners that differ in exactly one coordinate
EDGES = [(i, j) for i in range(8) for j in range(i + 1, 8)
         if sum(a != b for a, b in zip(VERTS[i], VERTS[j])) == 1]

L, R, B, T = -2, 2, -2, 2        # view window: x in [-2,2], y in [-2,2]
W, H = 800, 600                  # screen size in pixels


def ortho(p):
    x, y, z = p
    return x, y                  # ignore z


def to_pixel(x, y):
    """Normalise to [0,1] then scale to the screen (pixel y grows downwards)."""
    xn = (x - L) / (R - L)
    yn = (y - B) / (T - B)
    return xn * W, (1 - yn) * H, xn, yn


# ---- Question 1 ----
x, y = ortho((1, 1, 1))
px, py, xn, yn = to_pixel(x, y)
print("Q1: pixel coordinates of (1, 1, 1)")
print(f"    x_ndc = {xn}, y_ndc = {yn}")
print(f"    pixel = ({px:.0f}, {py:.0f})")

# ---- Question 2 ----
print("\nQ2: move the cube along z from 1 to 10")
for zc in (1, 10):
    moved = [(vx, vy, vz + zc) for vx, vy, vz in VERTS]
    pix = [to_pixel(*ortho(v))[:2] for v in moved]
    width = max(p[0] for p in pix) - min(p[0] for p in pix)
    height = max(p[1] for p in pix) - min(p[1] for p in pix)
    print(f"    cube centred at z={zc:>2}: projected size = {width:.0f} x {height:.0f} pixels")
print("    -> No change: orthographic projection ignores depth.")

# ---- Draw ----
pix = [to_pixel(*ortho(v))[:2] for v in VERTS]
fig, ax = plt.subplots(figsize=(8, 6))
for i, j in EDGES:
    back = VERTS[i][2] == -1 and VERTS[j][2] == -1
    ax.plot([pix[i][0], pix[j][0]], [pix[i][1], pix[j][1]],
            "r--" if back else "b-", lw=3 if not back else 1.5)
ax.plot([], [], "b-", label="front face (z = +1)")
ax.plot([], [], "r--", label="back face (z = -1)")
ax.scatter([pix[7][0]], [pix[7][1]], color="black", zorder=5)
ax.annotate("(1,1,1) -> (600,150)", pix[7], textcoords="offset points", xytext=(10, -15))
ax.set_xlim(0, W)
ax.set_ylim(H, 0)               # screen y grows downwards
ax.set_title("Orthographic projection: front and back faces overlap exactly")
ax.set_xlabel("pixel x")
ax.set_ylabel("pixel y")
ax.legend(loc="lower right")
ax.grid(True)

out_dir = os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "screenshots")
os.makedirs(out_dir, exist_ok=True)
plt.savefig(os.path.join(out_dir, "lab3_exp1_orthographic.png"), dpi=120)
plt.show()
