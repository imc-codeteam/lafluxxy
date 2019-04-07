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
    QPixmap pixmap = this->mz.generate_maze_pixmap(maze, 128);

    QLabel *mazelabel = new QLabel;
    mazelabel->setPixmap(pixmap);
    mazegrid->addWidget(mazelabel, row*2, col);

    QRadioButton* maze_select = new QRadioButton;
    this->maze_selector->addButton(maze_select);
    mazegrid->addWidget(maze_select, row*2 + 1, col);
}
