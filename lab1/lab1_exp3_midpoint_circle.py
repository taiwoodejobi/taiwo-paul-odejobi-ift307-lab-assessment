"""
Lab 1 - Experiment 3: Midpoint Circle Algorithm
Draw a circle of radius r centred at (h, k) and count the pixels
computed in the first octant only (the other 7 octants are mirrors).
"""
import os
import math
import matplotlib.pyplot as plt


def plot8(h, k, x, y, pixels):
    """8-way symmetry: one computed point gives 8 pixels on the circle."""
    pixels.extend([
        (h + x, k + y), (h - x, k + y), (h + x, k - y), (h - x, k - y),
        (h + y, k + x), (h - y, k + x), (h + y, k - x), (h - y, k - x),
    ])


def midpoint_circle(h, k, r):
    x = 0
    y = r
    d = 1 - r                      # initial decision variable
    octant = []                    # pixels actually computed (first octant)
    d_values = []
    all_pixels = []
    while x <= y:
        octant.append((h + x, k + y))
        d_values.append(d)
        plot8(h, k, x, y, all_pixels)
        if d < 0:                  # midpoint inside circle -> keep y
            d = d + 2 * x + 3
        else:                      # midpoint outside circle -> move y down
            d = d + 2 * x - 2 * y + 5
            y = y - 1
        x = x + 1
    return octant, d_values, all_pixels


h, k, r = 0, 0, 10
octant, d_values, all_pixels = midpoint_circle(h, k, r)

print(f"Midpoint circle: centre=({h},{k}), radius={r}\n")
print(" step   pixel      d")
for i, (pt, d) in enumerate(zip(octant, d_values)):
    print(f" {i:>3}   {str(pt):<9} {d:>4}")

unique_pixels = set(all_pixels)
print(f"\nPixels computed in first octant : {len(octant)}")
print(f"Total pixels on circle (8-way)  : {len(unique_pixels)}")
print(f"Work saved by symmetry          : only 1/8 of the circle is computed")

# ---- plot ----
fig, ax = plt.subplots(figsize=(7, 7))
t = [i * 2 * math.pi / 360 for i in range(361)]
ax.plot([h + r * math.cos(a) for a in t], [k + r * math.sin(a) for a in t],
        "r--", label="Ideal circle")
ax.scatter([p[0] for p in unique_pixels], [p[1] for p in unique_pixels],
           s=120, marker="s", color="lightsteelblue", label="Mirrored pixels (other 7 octants)")
ax.scatter([p[0] for p in octant], [p[1] for p in octant],
           s=120, marker="s", color="navy", label=f"First octant ({len(octant)} computed)")
ax.grid(True)
ax.set_aspect("equal")
ax.set_title(f"Midpoint Circle  r={r}, centre=({h},{k})")
ax.legend(loc="center", fontsize=8)

out_dir = os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "screenshots")
os.makedirs(out_dir, exist_ok=True)
plt.savefig(os.path.join(out_dir, "lab1_exp3_midpoint_circle.png"), dpi=120)
plt.show()
