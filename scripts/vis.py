#!/usr/bin/env python3

import struct
import numpy as np
import matplotlib.pyplot as plt
import sys

# set boundaries for cmap
vmin1 = float(sys.argv[2])
vmax1 = float(sys.argv[3])
vmin2 = float(sys.argv[4])
vmax2 = float(sys.argv[5])

# open file
with open(sys.argv[1], "rb") as f:
    # read header
    char = f.read(1)
    line = ""
    lines = []
    while char:
        if char.decode("utf-8") == '\n':
            lines.append(line)
            if line == "end_header":
                break
            else:
                line = ""
        else:
            line += char.decode("utf-8")

        char = f.read(1)

    nframes = 0
    cols = 0
    rows =0

    # grab info from keyword-value pairs
    for line in lines:
        pieces = line.split('=')
        keyword = pieces[0].strip()

        if keyword == "nframes":
            nframes = int(pieces[1])

        if keyword == "rows":
            rows = int(pieces[1])

        if keyword == "columns":
            cols = int(pieces[1])

    print("%i x %i x %i" % (nframes, rows, cols))
    print("vmin1 = %f" % vmin1)
    print("vmax1 = %f" % vmax1)
    print("vmin2 = %f" % vmin2)
    print("vmax2 = %f" % vmax2)

    # read concentrations
    for i in range(0, nframes):
        a = np.fromfile(f, dtype=np.dtype("float64"), count=cols * rows)
        b = np.fromfile(f, dtype=np.dtype("float64"), count=cols * rows)

        ap = a.reshape((rows, cols))
        bp = b.reshape((rows, cols))

        fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(12,4))

        im1 = ax1.imshow(ap, origin='lower', interpolation='bicubic', vmin=vmin1, vmax=vmax1)
        plt.colorbar(im1, ax=ax1)

        im2 = ax2.imshow(bp, origin='lower', interpolation='bicubic', vmin=vmin2, vmax=vmax2, cmap='PiYG')
        plt.colorbar(im2, ax=ax2)

        ax1.set_title('Concentration A')
        ax2.set_title('Concentration B')
        filename = '%04i.png' % i
        print("Writing image: %s" % filename)
        plt.savefig(filename, dpi=72)
        plt.close()
