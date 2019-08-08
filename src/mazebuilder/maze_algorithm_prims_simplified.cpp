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

#include "maze_algorithm_prims_simplified.h"
#include <algorithm>    // std::find

/**
 * @brief      Constructs the object.
 */
MazeAlgorithmPrimsSimplified::MazeAlgorithmPrimsSimplified() {}

/**
 * @brief      Builds a maze.
 *
 * @param[in]  rows  Number of rows
 * @param[in]  cols  Number of columns
 *
 * @return     The maze.
 */
Maze* MazeAlgorithmPrimsSimplified::build_maze(unsigned int rows, unsigned int cols) {
    Maze* maze = new Maze(rows, cols, MazeAlgorithmType::ALGO_PRIMS_SIMPLIFIED);
    unsigned int cell_row = rows / 2;
    unsigned int cell_col = cols / 2;
    std::vector<unsigned int> incorporated {cell_row * cols + cell_col};
    Cell* cell;

    while (incorporated.size() > 0) {
        unsigned int index = this->get_random_number_uint(incorporated.size() - 1);

        unsigned int cell_number = incorporated[index];
        unsigned int row = 0;
        while (cell_number >= cols) {
            cell_number = cell_number - cols;
            row = row + 1;
        }
        cell = maze->at(row, cell_number);

        std::vector<unsigned int> unlinked_neighbours;
        if(cell->get_north() != nullptr) {
            unlinked_neighbours.push_back(0);
        }
        if(cell->get_east() != nullptr) {
            unlinked_neighbours.push_back(1);
        }
        if(cell->get_south() != nullptr) {
            unlinked_neighbours.push_back(2);
        }
        if(cell->get_west() != nullptr) {
            unlinked_neighbours.push_back(3);
        }
        bool north = false; bool east = false; bool south = false; bool west = false;

        for (unsigned int i = 0; i < cell->get_neighbours().size(); i++) {
            for (auto const& x : cell->get_neighbours()[i]->get_links()) {
                if (x.second == true) {
                    if (unlinked_neighbours[i] == 0) {
                        north = true;
                    }
                    if (unlinked_neighbours[i] == 1) {
                        east = true;
                    }
                    if (unlinked_neighbours[i] == 2) {
                        south = true;
                    }
                    if (unlinked_neighbours[i] == 3) {
                        west = true;
                    }
                }
            }
        }

        std::vector<unsigned int>::iterator it;
        if (north == true) {
            it = std::find(unlinked_neighbours.begin(), unlinked_neighbours.end(), 0);
            unlinked_neighbours.erase(it);
        }

        if (east == true) {
            it = std::find(unlinked_neighbours.begin(), unlinked_neighbours.end(), 1);
            unlinked_neighbours.erase(it);
        }

        if (south == true) {
            it = std::find(unlinked_neighbours.begin(), unlinked_neighbours.end(), 2);
            unlinked_neighbours.erase(it);
        }

        if (west == true) {
            it = std::find(unlinked_neighbours.begin(), unlinked_neighbours.end(), 3);
            unlinked_neighbours.erase(it);
        }

        if (unlinked_neighbours.size() == 0) {
            it = std::find(incorporated.begin(), incorporated.end(), incorporated[index]);
            incorporated.erase(it);
            continue;
        }

        index = this->get_random_number_uint(unlinked_neighbours.size() - 1);

        if (unlinked_neighbours[index] == 0) {
            cell->link(cell->get_north());
            incorporated.push_back(cell->get_north()->row() * cols + cell->get_north()->col());
        }

        if (unlinked_neighbours[index] == 1) {
            cell->link(cell->get_east());
            incorporated.push_back(cell->get_east()->row() * cols + cell->get_east()->col());
        }

        if (unlinked_neighbours[index] == 2) {
            cell->link(cell->get_south());
            incorporated.push_back(cell->get_south()->row() * cols + cell->get_south()->col());
        }

        if (unlinked_neighbours[index] == 3) {
            cell->link(cell->get_west());
            incorporated.push_back(cell->get_west()->row() * cols + cell->get_west()->col());
        }
    }
    return maze;
}
