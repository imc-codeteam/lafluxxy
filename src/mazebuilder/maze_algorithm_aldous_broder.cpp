 /*************************************************************************
 *   This file is part of MazeBuilder                                     *
 *                                                                        *
 *   Authors: Ivo Filot <i.a.w.filot@tue.nl>                              *
 *            Roos Krösschell                                             *
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

#include "maze_algorithm_aldous_broder.h"

/**
 * @brief      Constructs the object.
 */
MazeAlgorithmAldousBroder::MazeAlgorithmAldousBroder() {}

/**
 * @brief      Builds a maze.
 *
 * @param[in]  rows  Number of rows
 * @param[in]  cols  Number of columns
 *
 * @return     The maze.
 */
Maze* MazeAlgorithmAldousBroder::build_maze(unsigned int rows, unsigned int cols) {
    Maze* maze = new Maze(rows, cols, MazeAlgorithmType::ALGO_ALDOUS_BRODER);
    unsigned int cell_row = this->get_random_number_uint(rows - 1);
    unsigned int cell_col = this->get_random_number_uint(cols - 1);
    Cell* cell = maze->at(cell_row, cell_col);

    unsigned int unvisited = rows * cols - 1;

    while (unvisited > 0) {
        bool visited = false;
        unsigned int index = this->get_random_number_uint(cell->get_neighbours().size() - 1);

        if (cell->get_neighbours()[index] == nullptr) {
            continue;
        }

        for (auto const& x : cell->get_neighbours()[index]->get_links()) {
            if (x.second == true) {
                visited = true;
            }
        }

        if (visited == false) {
            cell->link(cell->get_neighbours()[index]);
            unvisited = unvisited - 1;
        }
        cell = cell->get_neighbours()[index];
    }
    return maze;
}
