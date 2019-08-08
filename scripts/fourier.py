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

    # this information should be read from the .lfd file; at least
    # if a LaFluxxy version 0.8.0.0 or newer has been used
    nframes = 0
    cols = 0
    rows =0
    vmin1 = 1.0
    vmax1 = 4.0
    vmin2 = 2.0
    vmax2 = 6.0

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
    for i in range(0, nframes-1): # skip all non-last frames
        a = np.fromfile(f, dtype=np.dtype("float64"), count=cols * rows)
        b = np.fromfile(f, dtype=np.dtype("float64"), count=cols * rows)

    # read final frame
    a = np.fromfile(f, dtype=np.dtype("float64"), count=cols * rows)
    b = np.fromfile(f, dtype=np.dtype("float64"), count=cols * rows)
    ap = a.reshape((rows, cols))
    bp = b.reshape((rows, cols))
    
    # perform Fourier Transform
    fft_a = np.fft.fftshift(np.fft.fft2(ap))
    fft_b = np.fft.fftshift(np.fft.fft2(bp))
    
    fig, [[ax1, ax2], [ax3, ax4]] = plt.subplots(2, 2, figsize=(1920/dpi,1920/dpi))
    
    im1 = ax1.imshow(ap, origin='lower', interpolation='bicubic', vmin=vmin1, vmax=vmax1, cmap='Spectral')
    plt.colorbar(im1, ax=ax1)
    
    im2 = ax2.imshow(bp, origin='lower', interpolation='bicubic', vmin=vmin2, vmax=vmax2, cmap='PiYG')
    plt.colorbar(im2, ax=ax2)
    
    im3 = ax3.imshow(np.abs(fft_a), origin='lower', interpolation='bicubic', vmin=0, vmax=256, cmap='Spectral', extent=[-128,128,-128,128])
    ax3.grid(color='white', alpha=0.2)
    plt.colorbar(im3, ax=ax3)
    
    im4 = ax4.imshow(np.abs(fft_b), origin='lower', interpolation='bicubic', vmin=0, vmax=256, cmap='Spectral', extent=[-128,128,-128,128])
    ax4.grid(color='white', alpha=0.2)
    plt.colorbar(im4, ax=ax4)
    
    ax1.set_title('Concentration A')
    ax2.set_title('Concentration B')
    ax3.set_title('2D Fourier Transform A')
    ax4.set_title('2D Fourier Transform B')

    plt.savefig(sys.argv[2], dpi=dpi)
    plt.close()