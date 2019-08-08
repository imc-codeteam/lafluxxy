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

#include "maze_algorithm.h"

/**
 * @brief      Constructs the object.
 */
MazeAlgorithm::MazeAlgorithm() {}

/**
 * @brief      Get a random number on range [0,max]
 *
 * @param[in]  max  Maximum number
 *
 * @return     The random number
 */
unsigned int MazeAlgorithm::get_random_number_uint(unsigned int max) {
    static std::random_device dev;
    static std::mt19937 rng(dev());

    std::uniform_int_distribution<std::mt19937::result_type> dist(0,max);
    return dist(rng);
}

/**
 * @brief      Destroys the object.
 */
MazeAlgorithm::~MazeAlgorithm() {}
