import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

# 设置半径 R
R = 1

fig = plt.figure(figsize=(10, 8))
ax = fig.add_subplot(111, projection='3d')

# 生成球坐标参数
u = np.linspace(0, 2 * np.pi, 60)
v = np.linspace(0, np.pi, 60)
U, V = np.meshgrid(u, v)

# 球面 1: x^2 + y^2 + z^2 = R^2
x1 = R * np.cos(U) * np.sin(V)
y1 = R * np.sin(U) * np.sin(V)
z1 = R * np.cos(V)

# 球面 2: x^2 + y^2 + (z - R)^2 = R^2
x2 = R * np.cos(U) * np.sin(V)
y2 = R * np.sin(U) * np.sin(V)
z2 = R * np.cos(V) + R  # 中心向上平移 R

# 绘制球面 1 (半透明蓝色)
ax.plot_surface(x1, y1, z1, color='royalblue', alpha=0.4, edgecolor='k', linewidth=0.1)

# 绘制球面 2 (半透明红色)
ax.plot_surface(x2, y2, z2, color='crimson', alpha=0.4, edgecolor='k', linewidth=0.1)

# 绘制交线 (z = R/2, x^2 + y^2 = 3/4 R^2)
theta = np.linspace(0, 2 * np.pi, 100)
r_int = np.sqrt(R**2 - (R/2)**2)
ax.plot(r_int * np.cos(theta), r_int * np.sin(theta), R/2, color='yellow', lw=3, label='Intersection')

# 设置轴标签和透视视角
ax.set_xlabel('X')
ax.set_ylabel('Y')
ax.set_zlabel('Z')
ax.set_title(f'Solid Bounded by Two Spheres (R={R})')

# 调整视角以增强透视感 (elev:仰角, azim:方位角)
ax.view_init(elev=20, azim=45)

# 保持比例一致
ax.set_box_aspect([1, 1, 1.5]) 

plt.show()