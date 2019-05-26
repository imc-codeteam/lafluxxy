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

#include "maze.h"

/**
 * @brief      Constructs the object.
 *
 * @param[in]  _mrows  number of rows
 * @param[in]  _mcols  number of columns
 * @param[in]  _type   Algorithm used to generate this maze
 */
Maze::Maze(unsigned int _mrows, unsigned int _mcols, MazeAlgorithmType _type) :
mrows(_mrows),
mcols(_mcols),
algo_type(_type)
{
    // construct cells
    for(unsigned int row = 0; row < this->mrows; row++ ) {
        for(unsigned int col = 0; col < this->mcols; col++) {
            this->cells.emplace_back(row, col);
        }
    }

    // set neighbors
    #pragma omp parallel for
    for(int row = 0; row < (int)this->mrows; row++ ) {
        for(unsigned int col = 0; col < this->mcols; col++) {
            Cell* north = nullptr;
            Cell* east  = nullptr;
            Cell* south = nullptr;
            Cell* west  = nullptr;

            if(row != 0) {
                south = &this->cells[(row - 1) * this->mcols + col];
            }

            if(row < ((int)this->mrows - 1)) {
                north = &this->cells[(row + 1) * this->mcols + col];
            }

            if(col != 0) {
                west = &this->cells[row * this->mcols + col - 1];
            }

            if(col < (this->mcols - 1)) {
                east = &this->cells[row * this->mcols + col + 1];
            }

            this->cells[row * this->mcols + col].set_neighbours(north, east, south, west);
        }
    }
}

/**
 * @brief      Print matrix
 *
 * @param      os    Stream output
 * @param[in]  maze  Maze object
 *
 * @return     Stream output
 */
std::ostream& operator<<(std::ostream& os, const Maze& maze) {
    std::string output("+");

    for(unsigned int col = 0; col < maze.cols(); col++) {
        output += "---+";
    }

    output += "\n";

    for(int row = maze.rows() - 1; row >= 0; row--) {
        std::string top("|");
        std::string bottom("+");

        for(unsigned int col = 0; col < maze.cols(); col++) {
            std::string body("   ");
            const Cell& cell = maze(row, col);

            std::string east_boundary = (cell.is_linked(cell.get_east())) ? " " : "|";
            top += body + east_boundary;

            std::string south_boundary = (cell.is_linked(cell.get_south())) ? "   " : "---";
            std::string corner("+");
            bottom += south_boundary + corner;
        }

        output += top + "\n";
        output += bottom + "\n";
    }

    os << output;

    return os;
}

/**
 * @brief      Gets the mask.
 *
 * @param[in]  cell_size  The cell size
 *
 * @return     The mask.
 */
MatrixXXi Maze::get_mask(unsigned int cell_size) {
    MatrixXXi mask = MatrixXXi::Zero(this->mrows * cell_size, this->mcols * cell_size);

    // draw boundaries
    for(unsigned int i=0; i<this->mrows; i++) {
        for(unsigned int j=0; j<this->mcols; j++) {
            const Cell* cell = &this->cells[i * this->mcols + j];

            // southwest
            unsigned int x1 = j * cell_size;
            unsigned int y1 = i * cell_size;

            // northeast
            unsigned int x2 = (j+1) * cell_size - 1;
            unsigned int y2 = (i+1) * cell_size - 1;

            // draw cell borders
            if(!cell->is_linked(cell->get_north())) {
                for(unsigned int x=x1; x<=x2; x++) {
                    mask(y2, x) = 1;
                }
            }

            if(!cell->is_linked(cell->get_west())) {
                for(unsigned int y=y1; y<=y2; y++) {
                    mask(y, x1) = 1;
                }
            }

            if(!cell->is_linked(cell->get_east())) {
                for(unsigned int y=y1; y<=y2; y++) {
                    mask(y, x2) = 1;
                }
            }

            if(!cell->is_linked(cell->get_south())) {
                for(unsigned int x=x1; x<=x2; x++) {
                    mask(y1, x) = 1;
                }
            }
        }
    }

    return mask;
}
