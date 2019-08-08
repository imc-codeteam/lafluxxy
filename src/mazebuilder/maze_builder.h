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

#ifndef _MAZE_BUILDER_H
#define _MAZE_BUILDER_H

#include <memory>
#include <boost/core/noncopyable.hpp>

#include "maze_algorithm_binary_tree.h"
#include "maze_algorithm_sidewinder.h"
#include "maze_algorithm_aldous_broder.h"
#include "maze_algorithm_prims_simplified.h"

class MazeBuilder : private boost::noncopyable {
private:
    std::unique_ptr<MazeAlgorithm> maze_algo;

public:
    /**
     * @brief      Constructs the object.
     */
    MazeBuilder();

    /**
     * @brief      Builds a maze.
     *
     * @param[in]  algo  The algorithm
     * @param[in]  rows  Number of rows
     * @param[in]  cols  Number of columns
     *
     * @return     The maze.
     */
    Maze* build_maze(MazeAlgorithmType algo, unsigned int rows, unsigned int cols);

private:
};

#endif // _MAZE_BUILDER_H
