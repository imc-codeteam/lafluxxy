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

#ifndef _MAZE_ALGORITHM_BINARY_TREE_H
#define _MAZE_ALGORITHM_BINARY_TREE_H

#include "maze_algorithm.h"

class MazeAlgorithmBinaryTree : public MazeAlgorithm {
private:

public:
    /**
     * @brief      Constructs the object.
     */
    MazeAlgorithmBinaryTree();

    /**
     * @brief      Builds a maze.
     *
     * @param[in]  rows  Number of rows
     * @param[in]  cols  Number of columns
     *
     * @return     The maze.
     */
    Maze* build_maze(unsigned int rows, unsigned int cols) override;

    /**
     * @brief      Destroys the object.
     */
    ~MazeAlgorithmBinaryTree() override {}
private:

};

#endif // _MAZE_ALGORITHM_BINARY_TREE_H
