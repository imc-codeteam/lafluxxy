 /**************************************************************************
 *   This file is part of LaFluxxy                                        *
 *   https://github.com/imc-codeteam/lafluxxy                             *
 *                                                                        *
 *   Author: Ivo Filot <i.a.w.filot@tue.nl>                               *
 *                                                                        *
 *   LaFluxxy is free software: you can redistribute it and/or modify     *
 *   it under the terms of the GNU General Public License as published    *
 *   by the Free Software Foundation, either version 3 of the License,    *
 *   or (at your option) any later version.                               *
 *                                                                        *
 *   LaFluxxy is distributed in the hope that it will be useful,          *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty          *
 *   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.              *
 *   See the GNU General Public License for more details.                 *
 *                                                                        *
 *   You should have received a copy of the GNU General Public License    *
 *   along with this program.  If not, see http://www.gnu.org/licenses/.  *
 *                                                                        *
 **************************************************************************/

#include "mazetab.h"

/**
 * @brief Input tab constructor
 * @param parent widget
 */
MazeTab::MazeTab(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *mainLayout = new QVBoxLayout;
    this->setLayout(mainLayout);
    this->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

    // add a ScrollArea widget and define properties
    QScrollArea *scrollArea = new QScrollArea(this);     //Create scroll area Widget
    scrollArea->setContentsMargins(0,0,0,0);
    scrollArea->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    scrollArea->setWidgetResizable(true);

    // add ScrollArea to QWidget
    mainLayout->addWidget(scrollArea);

    // create new Widget for in the QScrollArea and set properties
    QWidget* widget = new QWidget();
    widget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    // add Widget to ScrollArea
    scrollArea->setWidget(widget);

    QVBoxLayout *layout = new QVBoxLayout;
    widget->setLayout(layout);

    // explanation label
    layout->addWidget(new QLabel(tr("Construct 2D mazes to introduce tortuosity in the simulation.")));

    // maze algo
    layout->addWidget(new QLabel(tr("<b>Maze generator algorithm</b>")));
    this->maze_algo_selector = new QComboBox();

    this->maze_algo_selector->addItem(tr("Please select an algorithm..."));
    this->maze_algo_selector->addItem(tr("Binary Tree"));
    this->maze_algo_selector->addItem(tr("Sidewinder"));
    this->maze_algo_selector->setCurrentIndex(0);
    layout->addWidget(this->maze_algo_selector, 1, 0);

    // maze results
    layout->addWidget(new QLabel(tr("<b>Maze results</b>")));

    QWidget* mazeholder = new QWidget;
    QGridLayout* mazegrid = new QGridLayout;
    mazeholder->setLayout(mazegrid);
    layout->addWidget(mazeholder);

    for(unsigned int y=0; y<3; y++) {
        for(unsigned int x=0; x<3; x++) {
            this->generate_maze(mazegrid, y, x);
        }
    }
}

void MazeTab::generate_maze(QGridLayout* mazegrid, unsigned int grow, unsigned int gcol) {
    unsigned int rows = 14;
    unsigned int cols = 14;
    Maze maze(rows, cols);
    maze.build_algo_binary_tree();
    maze.build_path_dijkstra(rows / 2, cols / 2);

    MazeRenderer mz;
    unsigned int cell_size = 16;
    std::vector<uint8_t> graph_data = mz.create_image(maze, cell_size);

    unsigned int img_width = maze.get_width() * cell_size + 1;
    unsigned int img_height = maze.get_height() * cell_size + 1;
    unsigned int mazewidth = img_width;
    unsigned int mazeheight = img_height;

    // Qt Images need to be 32-bits aligned
    img_width += ((img_width * 3) % 4);
    img_height += ((img_height * 3) % 4);

    QImage img(&graph_data[0], img_width, img_height, QImage::Format_RGB888);
    QImage cropped = img.copy(0, (3 - mazeheight % 4), mazewidth, mazeheight + (2 - mazeheight % 4));
    QPixmap pixmap = QPixmap::fromImage(cropped);
    pixmap = pixmap.scaled(img.width() * 2, img.height() * 2, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    QLabel *mazelabel = new QLabel;
    mazelabel->setPixmap(pixmap);
    mazegrid->addWidget(mazelabel, grow, gcol);
}
