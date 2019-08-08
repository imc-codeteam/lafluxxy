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

#ifndef _MAZE_ALGO_TYPES
#define _MAZE_ALGO_TYPES

#include <unordered_map>
#include <string>

/**
 * @brief      Enumeration class for maze algorithm type.
 */
enum class MazeAlgorithmType {
    ALGO_BINARY_TREE,
    ALGO_SIDEWINDER,
    ALGO_ALDOUS_BRODER,
    ALGO_PRIMS_SIMPLIFIED,

    NUM_ALGO_TYPES
};

// load unordered map with all possible maze algorithm
const std::unordered_map<std::string, MazeAlgorithmType> maze_algorithm_types {
    {"binary_tree", MazeAlgorithmType::ALGO_BINARY_TREE},
    {"sidewinder", MazeAlgorithmType::ALGO_SIDEWINDER},
    {"aldous_broder", MazeAlgorithmType::ALGO_ALDOUS_BRODER},
    {"prims_simplified", MazeAlgorithmType::ALGO_PRIMS_SIMPLIFIED}
};

const std::string& get_algo_name(MazeAlgorithmType algo_type);


#endif // _MAZE_ALGO_TYPES
