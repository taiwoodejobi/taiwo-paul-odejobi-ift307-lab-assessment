import os
import matplotlib.pyplot as plt

def bresenham(x1, y1, x2, y2):
    dx = abs(x2 - x1); dy = abs(y2 - y1)
    p = 2*dy - dx
    x = x1; y = y1
    points = []
    p_values = []
    while x < x2:
        points.append((x, y))
        p_values.append(p)
        x = x + 1
        if p >= 0:
            y = y + 1
            p = p + 2*dy - 2*dx
        else:
            p = p + 2*dy
    points.append((x, y))          # the final pixel (x2, y2)
    return points, p_values


points, p_values = bresenham(2, 3, 10, 8)
for pt, p in zip(points, p_values):
    print(pt, "  p =", p)
print("Last pixel:", points[-1])
print("Floating-point operations: 0 (integer only)")

xs = [p[0] for p in points]
ys = [p[1] for p in points]

plt.plot([2, 10], [3, 8], "r--", label="Ideal line")          # the true line
plt.scatter(xs, ys, s=300, marker="s", label="Bresenham pixels")      # the pixels chosen
plt.grid(True)
plt.gca().set_aspect("equal")
plt.title("Bresenham Line (2,3) → (10,8)   |   integer-only, 0 float ops")
plt.legend()

out_dir = os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "screenshots")
os.makedirs(out_dir, exist_ok=True)
plt.savefig(os.path.join(out_dir, "lab1_exp2_bresenham.png"), dpi=120)
plt.show()