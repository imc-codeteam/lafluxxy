#-------------------------------------------------
#
# Project created by QtCreator 2018-03-18T15:17:16
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = lafluxxy
TEMPLATE = app
CONFIG += c++14

gcc {
  QMAKE_CXXFLAGS += -fopenmp -march=native -Wall -Wextra
  QMAKE_LFLAGS += -fopenmp -march=native
  QMAKE_CXXFLAGS_RELEASE -= -O2
  QMAKE_CXXFLAGS_RELEASE *= -O3
}

msvc {
  QMAKE_CXXFLAGS += -openmp
  QMAKE_CXXFLAGS_RELEASE *= -O2
  RC_FILE = lafluxxy.rc
}

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += src/main.cpp\
           src/mainwindow.cpp \
           src/inputtab.cpp \
           src/resultstab.cpp \
           src/mazetab.cpp \
           src/input_reaction.cpp \
           src/input_lotka_volterra.cpp \
           src/input_brusselator.cpp \
           src/input_barkley.cpp \
           src/input_fitzhugh_nagumo.cpp \
           src/input_gray_scott.cpp \
           src/renderarea.cpp \
           src/two_dim_rd.cpp \
           src/reaction_lotka_volterra.cpp \
           src/reaction_gray_scott.cpp \
           src/reaction_brusselator.cpp \
           src/reaction_barkley.cpp \
           src/reaction_fitzhugh_nagumo.cpp \
           src/reaction_system.cpp \
           src/worker_thread.cpp \
           src/mazerenderer.cpp \
           src/mazeholder.cpp \
           src/movietab.cpp \
           src/colorscheme.cpp

HEADERS  += src/mainwindow.h \
            src/config.h \
            src/inputtab.h \
            src/resultstab.h \
            src/mazetab.h \
            src/input_reaction.h \
            src/input_lotka_volterra.h \
            src/input_barkley.h \
            src/input_brusselator.h \
            src/input_fitzhugh_nagumo.h \
            src/input_gray_scott.h \
            src/renderarea.h \
            src/two_dim_rd.h \
            src/reaction_gray_scott.h \
            src/reaction_lotka_volterra.h \
            src/reaction_brusselator.h \
            src/reaction_barkley.h \
            src/reaction_fitzhugh_nagumo.h \
            src/reaction_system.h \
            src/worker_thread.h \
            src/mazerenderer.h \
            src/mazeholder.h \
            src/matrices.h \
            src/movietab.h \
            src/colorscheme.h

# files from external library mazebuilder
SOURCES += vendor/mazebuilder/src/cell.cpp \
           vendor/mazebuilder/src/maze.cpp \
           vendor/mazebuilder/src/maze_algorithm.cpp \
           vendor/mazebuilder/src/maze_algorithm_aldous_broder.cpp \
           vendor/mazebuilder/src/maze_algorithm_binary_tree.cpp \
           vendor/mazebuilder/src/maze_algorithm_prims_simplified.cpp \
           vendor/mazebuilder/src/maze_algorithm_sidewinder.cpp \
           vendor/mazebuilder/src/maze_builder.cpp \
           vendor/mazebuilder/src/maze_statistics.cpp

# files from external library mazebuilder
HEADERS += vendor/mazebuilder/src/cell.h \
           vendor/mazebuilder/src/maze.h \
           vendor/mazebuilder/src/maze_algorithm.h \
           vendor/mazebuilder/src/maze_algorithm_aldous_broder.h \
           vendor/mazebuilder/src/maze_algorithm_binary_tree.h \
           vendor/mazebuilder/src/maze_algorithm_prims_simplified.h \
           vendor/mazebuilder/src/maze_algorithm_sidewinder.h \
           vendor/mazebuilder/src/maze_builder.h \
           vendor/mazebuilder/src/maze_statistics.h

# cuda sources
CUDA_SOURCES += src/card_manager.cu

# cuda headers
HEADERS += src/card_manager.h

unix {
    # set cuda stuff
    CUDA_DIR = /usr/local/cuda-10.0
    CUDA_LIBS =
    CUDA_LIBRT = /usr/local/cuda-10.0/lib64/libcudart_static.a
    # CUDA_ARCH = compute_52
    CUDA_INC = $$join($$CUDA_DIR/include,' -I','-I',' ')
    NVCCFLAGS = -use_fast_math --compile -cudart static

    cuda.commands = $$CUDA_DIR/bin/nvcc -m64 -c $$NVCCFLAGS $$CUDA_INC $$LIBS  ${QMAKE_FILE_NAME} -o ${QMAKE_FILE_OUT}
    cuda.dependency_type = TYPE_C
    cuda.depend_command = $$CUDA_DIR/bin/nvcc -M $$CUDA_INC $$NVCCFLAGS ${QMAKE_FILE_NAME}

    # other libraries
    INCLUDEPATH += $$CUDA_DIR/include /usr/include/eigen3
    LIBS += $$CUDA_LIBS $$CUDA_LIBRT -lboost_filesystem -lboost_system -ldl -lrt
    QMAKE_LIBDIR += $$CUDA_DIR/lib64
}

win32 {
    INCLUDEPATH += ../../../Libraries/boost-1.64.0-win-x64/include
    INCLUDEPATH += ../../../Libraries/eigen-3.3.3-win-x64
}

cuda.input = CUDA_SOURCES
cuda.output = ${OBJECTS_DIR}${QMAKE_FILE_BASE}_cuda.o
QMAKE_EXTRA_UNIX_COMPILERS += cuda
