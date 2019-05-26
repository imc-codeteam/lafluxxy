 /*************************************************************************
 *   This file is part of MazeBuilder                                     *
 *                                                                        *
 *   Authors: Ivo Filot <i.a.w.filot@tue.nl>                              *
 *            Roos Krösschell                                              *
 *                                                                        *
 *   MazeBuilder is free software: you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published    *
 *   by the Free Software Foundation, either version 3 of the License,    *
 *   or (at your option) any later version.                               *
 *                                                                        *
 *   MazeBuilder is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty          *
 *   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.              *
 *   See the GNU General Public License for more details.                 *
 *                                                                        *
 *   You should have received a copy of the GNU General Public License    *
 *   along with this program.  If not, see http://www.gnu.org/licenses/.  *
 *                                                                        *
 **************************************************************************/

#include "maze_algorithm_sidewinder.h"

/**
 * @brief      Constructs the object.
 */
MazeAlgorithmSidewinder::MazeAlgorithmSidewinder() {}

/**
 * @brief      Builds a maze.
 *
 * @param[in]  rows  Number of rows
 * @param[in]  cols  Number of columns
 *
 * @return     The maze.
 */
Maze* MazeAlgorithmSidewinder::build_maze(unsigned int rows, unsigned int cols) {
    Maze* maze = new Maze(rows, cols, MazeAlgorithmType::ALGO_SIDEWINDER);
    bool north;

    for(unsigned int row = 0; row < maze->rows(); row++) {
        std::vector<Cell*> run;  // stores current run

        for(unsigned int col = 0; col < maze->cols(); col++) {
            Cell* cell = maze->at(row, col);
            run.push_back(cell);
            std::vector<Cell*> neighbours;

            if(cell->get_east() != nullptr) {
                neighbours.push_back(cell->get_east());
                north = false;
            }

            if(cell->get_north() != nullptr) {
                neighbours.push_back(cell->get_north());
                north = true;
            }

            if(neighbours.size() == 0) {
                run.clear();
                continue;
            }

            unsigned int index = this->get_random_number_uint(neighbours.size() - 1);

            if (((neighbours.size() == 2) && (index == 0)) || ((neighbours.size() == 1) && (north == false))) {
                cell->link(neighbours[0]);
            }

            if (((neighbours.size() == 2) && (index == 1)) || ((neighbours.size() == 1) && (north == true))) {
                unsigned int index_run = this->get_random_number_uint(run.size() - 1);
                run[index_run]->link(run[index_run]->get_north());
                run.clear();
            }
        }
    }
    return maze;
}
