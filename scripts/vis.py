#!/usr/bin/env python3

import struct
import numpy as np
import matplotlib.pyplot as plt
import sys

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
    vmin1 = 0
    vmax1 = 0
    vmin2 = 0
    vmax2 = 0

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

        if keyword == "vmin1":
            vmin1 = float(pieces[1])

        if keyword == "vmin2":
            vmin2 = float(pieces[1])

        if keyword == "vmax1":
            vmax1 = float(pieces[1])

        if keyword == "vmax2":
            vmax2 = float(pieces[1])

    print("%i x %i x %i" % (nframes, rows, cols))
    print("vmin1 = %f" % vmin1)
    print("vmax1 = %f" % vmax1)
    print("vmin2 = %f" % vmin2)
    print("vmax2 = %f" % vmax2)

    # read concentrations
    dpi = 72*2
    for i in range(0, nframes):
        a = np.fromfile(f, dtype=np.dtype("float64"), count=cols * rows)
        b = np.fromfile(f, dtype=np.dtype("float64"), count=cols * rows)

        ap = a.reshape((rows, cols))
        bp = b.reshape((rows, cols))

        fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(1920/dpi,(1920/12*4)/dpi))

        im1 = ax1.imshow(ap, origin='lower', interpolation='bicubic', vmin=vmin1, vmax=vmax1, cmap='Spectral')
        plt.colorbar(im1, ax=ax1)

        im2 = ax2.imshow(bp, origin='lower', interpolation='bicubic', vmin=vmin2, vmax=vmax2, cmap='PiYG')
        plt.colorbar(im2, ax=ax2)

        ax1.set_title('Concentration A')
        ax2.set_title('Concentration B')
        filename = '%04i.png' % i
        print("Writing image: %s" % filename)
        plt.savefig(filename, dpi=dpi)
        plt.close()
