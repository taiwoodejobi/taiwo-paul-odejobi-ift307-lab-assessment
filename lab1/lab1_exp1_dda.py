import os
import matplotlib.pyplot as plt

def dda(x1, y1, x2, y2):
    dx = x2 - x1
    dy = y2 - y1
    steps = max(abs(dx), abs(dy))

    x_inc = dx / steps          # float op 1
    y_inc = dy / steps          # float op 2
    flops = 2

    x, y = x1, y1
    points = []
    for i in range(steps + 1):
        points.append((round(x), round(y)))
        if i < steps:               # no update needed after the last pixel
            x = x + x_inc           # float op
            y = y + y_inc           # float op
            flops += 2

    return points, flops


points, flops = dda(2, 3, 10, 8)
for p in points:
    print(p)
print("Floating-point operations:", flops, " (formula 2 + 2*steps =", 2 + 2 * (len(points) - 1), ")")

xs = [p[0] for p in points]
ys = [p[1] for p in points]

plt.plot([2, 10], [3, 8], "r--", label="Ideal line")          # the true line
plt.scatter(xs, ys, s=300, marker="s", label="DDA pixels")      # the pixels chosen
plt.grid(True)
plt.gca().set_aspect("equal")
plt.title(f"DDA Line (2,3) → (10,8)   |   float ops = {flops}")
plt.legend()

out_dir = os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "screenshots")
os.makedirs(out_dir, exist_ok=True)
plt.savefig(os.path.join(out_dir, "lab1_exp1_dda.png"), dpi=120)
plt.show()
