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

#ifndef _MAZE_ALGORITHM_H
#define _MAZE_ALGORITHM_H

#include <boost/core/noncopyable.hpp>
#include <random>

#include "maze_algo_types.h"
#include "maze.h"

class MazeAlgorithm : private boost::noncopyable{
private:

public:
    /**
     * @brief      Constructs the object.
     */
    MazeAlgorithm();

    /**
     * @brief      Builds a maze.
     *
     * @param[in]  rows  Number of rows
     * @param[in]  cols  Number of columns
     *
     * @return     The maze.
     */
    virtual Maze* build_maze(unsigned int rows, unsigned int cols) = 0;

    /**
     * @brief      Destroys the object.
     */
    virtual ~MazeAlgorithm() = 0;

protected:
    /**
     * @brief      Get a random number on range [0,max]
     *
     * @param[in]  max  Maximum number
     *
     * @return     The random number
     */
    unsigned int get_random_number_uint(unsigned int max);
};

#endif // _MAZE_ALGORITHM_H
