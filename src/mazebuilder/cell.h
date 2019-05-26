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

#ifndef _CELL_H
#define _CELL_H

#include <vector>
#include <unordered_map>

class Cell {
private:
    unsigned int mrow;  //!< which row the cell is positioned in
    unsigned int mcol;  //!< which column the cell is positioned in

    Cell* north;        //!< cell in the north
    Cell* east;         //!< cell in the east
    Cell* south;        //!< cell in the south
    Cell* west;         //!< cell in the west

    std::vector<Cell*> neighbours;                      //!< neighbours of this cell; only the existing ones
    std::vector<Cell> unlinked_neighbours;
    std::unordered_map<const Cell*, bool> links;        //!< cells that are linked to this cell; only the existing ones

public:
    /**
     * @brief      Constructs the object.
     *
     * @param[in]  _row  The row
     * @param[in]  _col  The col
     */
    Cell(unsigned int _row, unsigned int _col);

    /*
     * SETTERS
     */

    /**
     * @brief      Sets the neighors.
     *
     * @param      _north  The north cell
     * @param      _east   The east cell
     * @param      _south  The south cell
     * @param      _west   The west cell
     */
    void set_neighbours(Cell* _north, Cell* _east, Cell* _south, Cell* _west);

    /**
     * @brief      Link two cells
     *
     * @param      cell  Cell to link to
     * @param[in]  bidi  Whether bidirectional
     */
    void link(Cell* cell, bool bidi = true);

    /**
     * @brief      Unlink two cells
     *
     * @param      cell  Cell to unlink
     * @param[in]  bidi  Whether bidirectional
     */
    void unlink(Cell* cell, bool bidi = true);

    /**
     * @brief      Get the linked cells
     *
     * @return     The linked cells
     */
    inline const auto& get_links() const {
        return this->links;
    }

    /**
     * @brief      Get the neighbour cells
     *
     * @return     The neighbour cells
     */
    inline std::vector<Cell*> get_neighbours() const {
        return this->neighbours;
    }

    /*
     * GETTERS
     */

    /**
     * @brief      Get the row wherein this cell resides
     *
     * @return     Row
     */
    inline unsigned int row() const {
        return this->mrow;
    }

    /**
     * @brief      Get the column wherein this cell resides
     *
     * @return     Column
     */
    inline unsigned int col() const {
        return this->mcol;
    }

    /**
     * @brief      Get cell on the east
     *
     * @return     east cell
     */
    inline Cell* get_east() const {
        return this->east;
    }

    /**
     * @brief      Get cell on the north
     *
     * @return     north cell
     */
    inline Cell* get_north() const {
        return this->north;
    }

    /**
     * @brief      Get cell on the south
     *
     * @return     south cell
     */
    inline Cell* get_south() const {
        return this->south;
    }

    /**
     * @brief      Get cell on the west
     *
     * @return     west cell
     */
    inline Cell* get_west() const {
        return this->west;
    }

    /**
     * @brief      Determines if linked.
     *
     * @param[in]  cell  The cell
     *
     * @return     True if linked, False otherwise.
     */
    bool is_linked(const Cell* cell) const;

private:
};

#endif // _CELL_H
