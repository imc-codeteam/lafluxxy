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

#include "maze_builder.h"

/**
 * @brief      Constructs the object.
 */
MazeBuilder::MazeBuilder() {}

/**
 * @brief      Builds a maze.
 *
 * @param[in]  algo  The algorithm
 * @param[in]  rows  Number of rows
 * @param[in]  cols  Number of columns
 *
 * @return     The maze.
 */
Maze* MazeBuilder::build_maze(MazeAlgorithmType algo, unsigned int rows, unsigned int cols) {
    // clean up algorithm class if pointer is set
    if(this->maze_algo) {
        this->maze_algo.release();
    }

    // create new pointer to algorithm
    switch(algo) {
        case MazeAlgorithmType::ALGO_BINARY_TREE:
            this->maze_algo = std::unique_ptr<MazeAlgorithm>(reinterpret_cast<MazeAlgorithm*>(new MazeAlgorithmBinaryTree()));
        break;
        case MazeAlgorithmType::ALGO_SIDEWINDER:
            this->maze_algo = std::unique_ptr<MazeAlgorithm>(reinterpret_cast<MazeAlgorithm*>(new MazeAlgorithmSidewinder()));
        break;
        case MazeAlgorithmType::ALGO_ALDOUS_BRODER:
            this->maze_algo = std::unique_ptr<MazeAlgorithm>(reinterpret_cast<MazeAlgorithm*>(new MazeAlgorithmAldousBroder()));
        break;
        case MazeAlgorithmType::ALGO_PRIMS_SIMPLIFIED:
            this->maze_algo = std::unique_ptr<MazeAlgorithm>(reinterpret_cast<MazeAlgorithm*>(new MazeAlgorithmPrimsSimplified()));
        break;
        default:
            throw std::runtime_error("Invalid algorithm requested.");
        break;
    }

    return this->maze_algo->build_maze(rows, cols);
}
