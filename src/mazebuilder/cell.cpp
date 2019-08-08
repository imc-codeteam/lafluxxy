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

#include "cell.h"

/**
 * @brief      Constructs the object.
 *
 * @param[in]  _row  The row
 * @param[in]  _col  The col
 */
Cell::Cell(unsigned int _row, unsigned int _col) : mrow(_row), mcol(_col)
{

}

/**
 * @brief      Sets the neighors.
 *
 * @param      _north  The north cell
 * @param      _east   The east cell
 * @param      _south  The south cell
 * @param      _west   The west cell
 */
void Cell::set_neighbours(Cell* _north, Cell* _east, Cell* _south, Cell* _west) {
    this->north = _north;
    this->east = _east;
    this->south = _south;
    this->west = _west;

    if(this->north != nullptr) {
        this->links.emplace(this->north, false);
        this->neighbours.push_back(this->north);
    }

    if(this->east != nullptr) {
        this->links.emplace(this->east, false);
        this->neighbours.push_back(this->east);
    }

    if(this->south != nullptr) {
        this->links.emplace(this->south, false);
        this->neighbours.push_back(this->south);
    }

    if(this->west != nullptr) {
        this->links.emplace(this->west, false);
        this->neighbours.push_back(this->west);
    }
}

/**
 * @brief      Link two cells
 *
 * @param      cell  Cell to link to
 * @param[in]  bidi  Whether bidirectional
 */
void Cell::link(Cell* cell, bool bidi) {
    auto got = this->links.find(cell);
    if(got != this->links.end()) {
        got->second = true;
        if(bidi) {
            cell->link(this, false);
        }
    } else {
        throw std::runtime_error("Invalid cell link request received.");
    }
}

/**
 * @brief      Unlink two cells
 *
 * @param      cell  Cell to unlink
 * @param[in]  bidi  Whether bidirectional
 */
void Cell::unlink(Cell* cell, bool bidi) {
    auto got = this->links.find(cell);
    if(got != this->links.end()) {
        got->second = false;
        if(bidi) {
            cell->unlink(this, false);
        }
    } else {
        throw std::runtime_error("Invalid cell link request received.");
    }
}

/**
 * @brief      Determines if linked.
 *
 * @param[in]  cell  The cell
 *
 * @return     True if linked, False otherwise.
 */
bool Cell::is_linked(const Cell* cell) const {
    if(cell == nullptr) {
        return false;
    }

    auto got = this->links.find(cell);
    if(got != this->links.end()) {
        return got->second;
    } else {
        return false;
    }
}
