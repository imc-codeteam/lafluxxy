 /**************************************************************************
 *   This file is part of LaFluxxy                                        *
 *   https://github.com/imc-codeteam/lafluxxy                             *
 *                                                                        *
 *   Author: Ivo Filot <i.a.w.filot@tue.nl>                               *
 *                                                                        *
 *   LaFluxxy is free software: you can redistribute it and/or modify     *
 *   it under the terms of the GNU General Public License as published    *
 *   by the Free Software Foundation, either version 3 of the License,    *
 *   or (at your option) any later version.                               *
 *                                                                        *
 *   LaFluxxy is distributed in the hope that it will be useful,          *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty          *
 *   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.              *
 *   See the GNU General Public License for more details.                 *
 *                                                                        *
 *   You should have received a copy of the GNU General Public License    *
 *   along with this program.  If not, see http://www.gnu.org/licenses/.  *
 *                                                                        *
 **************************************************************************/

#ifndef _MAZE_H
#define _MAZE_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include <random>
#include <QByteArray>

class Cell {
private:
    unsigned int row;
    unsigned int column;

    Cell* north;
    Cell* south;
    Cell* east;
    Cell* west;

    std::unordered_map<Cell*, bool> links;

public:
    /**
    * @brief      Constructs the object.
    *
    * @param[in]  _row     The row index
    * @param[in]  _column  The column index
    */
    Cell(unsigned int _row, unsigned int _column);

    /**
     * @brief      Sets the neighbors.
     *
     * @param      _north  North neighbor
     * @param      _south  South neighbor
     * @param      _east   East neighbor
     * @param      _west   West neighbor
     */
    void set_neighbors(Cell* _north, Cell* _south, Cell* _east, Cell* _west);

    /**
     * @brief      Link a neighbor
     *
     * @param      neighbor     The neighbor
     * @param[in]  do_neighbor  whether to recursively link the neighbor
     */
    void link(Cell* neighbor, bool do_neighbor = true);

    /**
     * @brief      Unlink a neighbor
     *
     * @param      neighbor     The neighbor
     * @param[in]  do_neighbor  whether to recursively unlink the neighbor
     */
    void unlink(Cell* neighbor, bool do_neighbor = true);

    inline Cell* get_north() const {
        return this->north;
    }

    inline Cell* get_south() const {
        return this->south;
    }

    inline Cell* get_east() const {
        return this->east;
    }

    inline Cell* get_west() const {
        return this->west;
    }

    bool is_linked(Cell* neighbor) const;
};

class Maze {
private:
    unsigned int width;
    unsigned int height;

    std::vector<std::vector<Cell> > cells;

public:
    /**
     * @brief      Constructs the object.
     *
     * @param[in]  _width   The width
     * @param[in]  _height  The height
     */
    Maze(unsigned int _width, unsigned int _height);

    void build_algo_binary_tree();

    void print() const;

    QByteArray create_image(unsigned int cell_size = 10) const;

private:
    /**
     * @brief      Configure the neighbors of each cell
     */
    void configure_cells();

    unsigned int randint(unsigned int minval, unsigned int maxval) {
        static std::random_device dev;
        static std::mt19937 rng(dev());
        std::uniform_int_distribution<std::mt19937::result_type> dist(minval, maxval);

        return dist(rng);
    }
};

#endif // _MAZE_H
