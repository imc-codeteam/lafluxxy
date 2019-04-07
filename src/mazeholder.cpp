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

#include "mazeholder.h"

/**
 * @brief      Constructs the object.
 *
 * @param      parent  Parent Widget
 */
MazeHolder::MazeHolder(QWidget* parent) : QWidget(parent) {
    this->mazegrid = new QGridLayout;
    this->setLayout(mazegrid);
    this->maze_selector = new QButtonGroup;
}

/**
 * @brief      Builds mazes.
 *
 * @param[in]  mrows  Number of rows in mazes
 * @param[in]  mcols  Number of columns in mazes
 */
void MazeHolder::build_mazes(unsigned int mrows, unsigned int mcols) {
    for(unsigned int y=0; y<3; y++) {
        for(unsigned int x=0; x<3; x++) {
            this->generate_maze(this->mazegrid, mrows, mcols, y, x);
        }
    }
}

/**
 * @brief      Generate single maze
 *
 * @param      mazegrid  Target layout to place Maze image in
 * @param[in]  row       Row number
 * @param[in]  col       Column number
 */
void MazeHolder::generate_maze(QGridLayout* mazegrid, unsigned int rows, unsigned int cols, unsigned int row, unsigned int col) {
    this->mazes.push_back(std::make_unique<Maze>(rows, cols));
    this->mazes.back()->build_algo_binary_tree();
    this->mazes.back()->build_path_dijkstra(rows / 2, cols / 2);
    const Maze& maze = *this->mazes.back().get();

    unsigned int cell_size = 256 / std::max(rows, cols);
    std::vector<uint8_t> graph_data = this->mz.create_image(maze, cell_size);

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
    // pixmap = pixmap.scaled(img.width() * 2, img.height() * 2, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    QLabel *mazelabel = new QLabel;
    mazelabel->setPixmap(pixmap);
    mazegrid->addWidget(mazelabel, row*2, col);

    QRadioButton* maze_select = new QRadioButton;
    this->maze_selector->addButton(maze_select);
    mazegrid->addWidget(maze_select, row*2 + 1, col);
}
