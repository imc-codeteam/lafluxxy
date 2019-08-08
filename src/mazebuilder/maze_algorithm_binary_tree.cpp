 /*************************************************************************
 *   This file is part of MazeBuilder                                     *
 *                                                                        *
 *   Author: Ivo Filot <i.a.w.filot@tue.nl>                               *
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

#include "maze_algorithm_binary_tree.h"

/**
 * @brief      Constructs the object.
 */
MazeAlgorithmBinaryTree::MazeAlgorithmBinaryTree() {}

/**
 * @brief      Builds a maze.
 *
 * @param[in]  rows  Number of rows
 * @param[in]  cols  Number of columns
 *
 * @return     The maze.
 */
Maze* MazeAlgorithmBinaryTree::build_maze(unsigned int rows, unsigned int cols) {
    Maze* maze = new Maze(rows, cols, MazeAlgorithmType::ALGO_BINARY_TREE);

    for(unsigned int row = 0; row < maze->rows(); row++) {
        for(unsigned int col = 0; col < maze->cols(); col++) {
            Cell* cell = maze->at(row, col);
            std::vector<Cell*> neighbours;

            if(cell->get_north() != nullptr) {
                neighbours.push_back(cell->get_north());
            }

            if(cell->get_east() != nullptr) {
                neighbours.push_back(cell->get_east());
            }

            if(neighbours.size() == 0) {
                continue;
            }

            unsigned int index = this->get_random_number_uint(neighbours.size() -1);
            cell->link(neighbours[index]);
        }
    }

    return maze;
}
