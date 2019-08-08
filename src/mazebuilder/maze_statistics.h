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

#ifndef _MAZE_STATISTICS_H
#define _MAZE_STATISTICS_H

#include <boost/core/noncopyable.hpp>

#include "maze.h"
#include "matrices.h"

class MazeStatistics : private boost::noncopyable {
private:

public:
    /**
     * @brief      Constructs the object.
     */
    MazeStatistics();

    /**
     * @brief      Perform Dijkstra algorithm on maze
     *
     * @param[in]  startrow  Starting row
     * @param[in]  startcol  Starting column
     *
     * @return     Return vector of path lengths
     */
    MatrixXXi perform_dijkstra(const Maze& maze, unsigned int startrow, unsigned int startcol) const;


private:
};

#endif // _MAZE_STATISTICS_H
