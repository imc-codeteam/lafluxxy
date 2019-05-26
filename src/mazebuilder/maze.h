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

#ifndef _MAZE_H
#define _MAZE_H

#include <iostream>
#include <string>

#include "matrices.h"
#include "cell.h"
#include "maze_algo_types.h"

class Maze {
private:
    unsigned int mrows;                 //!< number of rows
    unsigned int mcols;                 //!< number of columns

    std::vector<Cell> cells;            //!< cells in the maze
    const MazeAlgorithmType algo_type;  //!< which algorithm is used to generate this maze

public:
    /**
     * @brief      Constructs the object.
     *
     * @param[in]  _mrows  number of rows
     * @param[in]  _mcols  number of columns
     * @param[in]  _type   Algorithm used to generate this maze
     */
    Maze(unsigned int _mrows, unsigned int _mcols, MazeAlgorithmType _type);

    /**
     * @brief      Gets the mask.
     *
     * @param[in]  cell_size  The cell size
     *
     * @return     The mask.
     */
    MatrixXXi get_mask(unsigned int cell_size);

    /**
     * @brief      Get number of rows
     *
     * @return     Number of rows
     */
    inline unsigned int rows() const {
        return this->mrows;
    }

    /**
     * @brief      Get number of cols
     *
     * @return     Number of cols
     */
    inline unsigned int cols() const {
        return this->mcols;
    }

    /**
     * @brief      Gets the maze generator algorithm type.
     *
     * @return     The algorithm type.
     */
    inline auto get_algo_type() const {
        return this->algo_type;
    }

    /**
     * @brief      Accessor
     *
     * @param[in]  row
     * @param[in]  column
     *
     * @return     Cell object
     */
    inline const Cell& operator()(unsigned int row, unsigned int col) const {
        return this->cells[row * this->mcols + col];
    }

    /**
     * @brief      Accessor
     *
     * @param[in]  row
     * @param[in]  column
     *
     * @return     Cell object
     */
    inline const Cell* at(unsigned int row, unsigned int col) const {
        return &this->cells[row * this->mcols + col];
    }

    /**
     * @brief      Accessor
     *
     * @param[in]  row
     * @param[in]  column
     *
     * @return     Cell object
     */
    inline Cell& operator()(unsigned int row, unsigned int col) {
        return this->cells[row * this->mcols + col];
    }

    /**
     * @brief      Accessor
     *
     * @param[in]  row
     * @param[in]  column
     *
     * @return     Cell object
     */
    inline Cell* at(unsigned int row, unsigned int col) {
        return &this->cells[row * this->mcols + col];
    }

private:
};

/**
 * @brief      Print matrix
 *
 * @param      os    Stream output
 * @param[in]  maze  Maze object
 *
 * @return     Stream output
 */
std::ostream& operator<<(std::ostream& os, const Maze& maze);

#endif // _MAZE_H
