# LaFluxxy

## Purpose
LaFluxxy is a simple GUI for performing reaction-diffusion integration on a rectangular grid, with and without mazes.

## Compilation instructions
```
mkdir build
cd build
qmake ../lafluxxy.pro
make -j9
```
## Dependencies

LaFluxxy depends on Mazebuilder, which is bundled using a git module. To initialize git modules:
```
git submodule init
```

and to update the modules
```
git submodule init
```

### Debian
```
sudo apt install qt5-default qt5-qmake libeigen3-dev libboost-all-dev
```
