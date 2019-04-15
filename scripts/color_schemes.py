import matplotlib.pyplot as plt
import matplotlib.cm as cm
import numpy as np

color_scheme = cm.get_cmap('PiYG')
x = np.linspace(0, 1, 256)
color_values = color_scheme(x)

f = open('scheme.txt', 'w+')
for val in color_values:
    f.write("                %7.6ff, %7.6ff, %7.6ff,\n" % (val[0], val[1], val[2]))
f.close()