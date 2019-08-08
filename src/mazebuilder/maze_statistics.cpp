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

#include "maze_statistics.h"

/**
 * @brief      Constructs the object.
 */
MazeStatistics::MazeStatistics() {

}

/**
 * @brief      Perform Dijkstra algorithm on maze
 *
 * @param[in]  startrow  Starting row
 * @param[in]  startcol  Starting column
 *
 * @return     Return vector of path lengths
 */
MatrixXXi MazeStatistics::perform_dijkstra(const Maze& maze, unsigned int startrow, unsigned int startcol) const {
    unsigned int mrows = maze.rows();
    unsigned int mcols = maze.cols();

    if(startrow >= mrows) {
        throw std::runtime_error("Invalid starting row requested");
    }

    if(startcol >= mcols) {
        throw std::runtime_error("Invalid starting column requested");
    }

    MatrixXXi result = MatrixXXi::Ones(mrows, mcols) * -1;

    const Cell* start = &maze(startrow, startcol);
    unsigned int path_length = 1;
    std::vector<const Cell*> frontier;
    result(startrow, startcol) = 0;

    // build initial frontier list
    for(const auto& it : start->get_links()) {
        if(it.first == nullptr || it.second == false) {
            continue;
        }
        frontier.push_back(it.first);
    }

    while(frontier.size() != 0) {
        std::vector<const Cell*> new_frontier;

        for(const Cell* cell : frontier) {
            result(cell->row(), cell->col()) = path_length;

            for(const auto& it : cell->get_links()) {

                if(it.first == nullptr || it.second == false) {
                    continue;
                }

                if(result(it.first->row(), it.first->col()) != -1) {
                    continue;
                }

                new_frontier.push_back(it.first);
            }
        }

        frontier = new_frontier;
        path_length++;
    }

    return result;
}
