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

    Maze maze(13,13);
    maze.build_algo_binary_tree();
    maze.build_path_dijkstra();

    MazeRenderer mz;
    unsigned int cell_size = 64;
    std::vector<uint8_t> graph_data = mz.create_image(maze, cell_size);

    unsigned int img_width = maze.get_width() * cell_size + 1;
    unsigned int img_height = maze.get_height() * cell_size + 1;
    unsigned int mazewidth = img_width;
    unsigned int mazeheight = img_height;

    // Qt Images need to be 32-bits aligned
    img_width += (4 - img_width % 4);
    img_height += (4 - img_height % 4);

    QImage img(&graph_data[0], img_width, img_height, QImage::Format_Grayscale8);
    QImage cropped = img.copy(0, (3 - mazeheight % 4), mazewidth, mazeheight + (2 - mazeheight % 4));
    QPixmap pixmap = QPixmap::fromImage(cropped);

    QLabel *mazelabel = new QLabel;
    mazelabel->setPixmap(pixmap);
    layout->addWidget(mazelabel);
}
