import matplotlib.pyplot as plt
from matplotlib.pyplot import MultipleLocator


plotx_list = [[1, 2, 3, 4, 5, 6, 7, 8, 9, 10]]

ploty_list_add = [
    [2.277, 4.505, 6.270, 9.003, 11.751, 13.491, 15.928, 18.098, 20.868, 22.614],
    [2.000, 4.005, 6.010, 8.017, 10.022, 12.028, 14.035, 16.041, 18.046, 20.055],
]

ploty_list_del = [
    [3.104, 6.208, 9.305, 12.401, 15.494, 18.588, 21.671, 24.754, 27.832, 30.901],
    [2.252, 4.520, 6.964, 9.345, 11.716, 14.010, 16.376, 18.650, 20.926, 23.170],
]

ploty_list_delay = [
    [5161, 6012, 6697, 7095, 7429, 7731, 8153, 8321, 8609, 8744],
    [3627, 3891, 3932, 4214, 4330, 4481, 4577, 4663, 4793, 4888],
]

ploty_list_mode_i7 = [
    [188, 184, 184, 188, 213],
    [188, 186, 184, 188, 213],
    [346, 350, 352, 383, 350],
    [346, 350, 352, 346, 350],
    [512, 512, 516, 534, 526],
    [512, 512, 516, 532, 526],
]

ploty_list_average_i7 = [
    [216.2, 215.9, 229.3, 203.1, 247.0],
    [216.9, 214.9, 230.6, 200.5, 267.0],
    [377.1, 380.0, 373.0, 420.1, 380.5],
    [376.2, 380.3, 372.4, 385.4, 379.1],
    [544.8, 546.4, 532.6, 563.6, 548.4],
    [543.8, 545.6, 532.0, 562.1, 546.4]
]

for i in range(len(ploty_list_mode_i7)):
    for j in range(len(ploty_list_mode_i7[i])):
        ploty_list_mode_i7[i][j] = ploty_list_mode_i7[i][j] - 91

for i in range(len(ploty_list_average_i7)):
    for j in range(len(ploty_list_average_i7[i])):
        ploty_list_average_i7[i][j] = ploty_list_average_i7[i][j] - 91

plt.rcParams["font.family"] = "Times New Roman"
plt.rcParams["font.size"] = 16
plt.rcParams['font.weight'] = "bold"
plt.rcParams['axes.labelweight'] = "bold"

fig1 = plt.figure(figsize=(8, 4))
ax = plt.axes()

color1 = '#5B84B1FF'
color2 = '#FC766AFF'
color3 = '#8EC260'
#ax.plot(plotx_list[0], ploty_list_del[0], color=color1, marker='o', ms=7,  mec=color1, mew=1.5, mfc='white', linestyle='-', label='Binary Heap Delete')
#ax.plot(plotx_list[0], ploty_list_del[1], color=color1, marker='o', ms=7,  mec=color1, mew=1.5, mfc='white', linestyle='--', label='Fibonacci Heap Delete')
#ax.plot(plotx_list[0], ploty_list_del[0], color=color2, marker='D', ms=6, mec=color2, mew=1.5, mfc='white', linestyle='-', label='2 accesses, secret 0')
#ax.plot(plotx_list[0], ploty_list_del[1], color=color2, marker='D', ms=6, mec=color2, mew=1.5, mfc='white', linestyle='--', label='2 accesses, secret 1')
ax.plot(plotx_list[0], ploty_list_delay[0], color=color1, marker='o', ms=7,  mec=color1, mew=1.5, mfc='white', linestyle='-', label='Binary Heap Delay')
ax.plot(plotx_list[0], ploty_list_delay[1], color=color1, marker='o', ms=7,  mec=color1, mew=1.5, mfc='white', linestyle='--', label='Fibonacci Heap Delay')



ax.legend(bbox_to_anchor=(0.014, 1), loc=3, borderaxespad=0, ncol=3, fontsize='small', frameon=False,
               columnspacing=0.5)
ax.set_axisbelow(True)


x_major_locator = MultipleLocator(1)
ax.xaxis.set_major_locator(x_major_locator)
plt.xlabel("Number of Nodes in Network (thousand)")
plt.ylabel("Max Delay (ms)")

plt.savefig('delay.pdf', format='pdf', dpi=1000, bbox_inches='tight')
# plt.show()