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

#include "maze.h"

/**
 * @brief      Constructs the object.
 *
 * @param[in]  _row     The row index
 * @param[in]  _column  The column index
 */
Cell::Cell(unsigned int _row, unsigned int _column) : row(_row), column(_column) {
    this->north = nullptr;
    this->south = nullptr;
    this->east = nullptr;
    this->west = nullptr;
}

/**
 * @brief      Sets the neighbors.
 *
 * @param      _north  North neighbor
 * @param      _south  South neighbor
 * @param      _east   East neighbor
 * @param      _west   West neighbor
 */
void Cell::set_neighbors(Cell* _north, Cell* _south, Cell* _east, Cell* _west) {
    this->north = _north;
    this->south = _south;
    this->east = _east;
    this->west = _west;

    this->links.emplace(this->north, false);
    this->links.emplace(this->south, false);
    this->links.emplace(this->east, false);
    this->links.emplace(this->west, false);
}

/**
 * @brief      Link a neighbor
 *
 * @param      neighbor     The neighbor
 * @param[in]  do_neighbor  whether to recursively link the neighbor
 */
void Cell::link(Cell* neighbor, bool do_neighbor) {
    auto got = this->links.find(neighbor);
    if(got != this->links.end()) {
        got->second = true;
    } else {
        throw std::runtime_error("Invalid link request");
    }

    if(do_neighbor) {
        neighbor->link(this, false);
    }
}

/**
 * @brief      Link a neighbor
 *
 * @param      neighbor     The neighbor
 * @param[in]  do_neighbor  whether to recursively link the neighbor
 */
void Cell::unlink(Cell* neighbor, bool do_neighbor) {
    auto got = this->links.find(neighbor);
    if(got != this->links.end()) {
        got->second = true;
    } else {
        throw std::runtime_error("Invalid unlink request");
    }

    if(do_neighbor) {
        neighbor->unlink(this, false);
    }
}

bool Cell::is_linked(Cell* neighbor) const {
    auto got = this->links.find(neighbor);
    return got->second;
}

/**
 * @brief      Constructs the object.
 *
 * @param[in]  _width   The width
 * @param[in]  _height  The height
 */
Maze::Maze(unsigned int _width, unsigned int _height) : width(_width), height(_height) {
    for(unsigned int i=0; i<this->height; i++) {
        this->cells.emplace_back();
        for(unsigned int j=0; j<this->width; j++) {
            this->cells[i].emplace_back(i,j);
        }
    }
    this->configure_cells();
}

void Maze::build_algo_binary_tree() {
    for(unsigned int i=0; i<this->height; i++) {
        for(unsigned int j=0; j<this->width; j++) {
            std::vector<Cell*> neighbors;

            if(this->cells[i][j].get_north() != nullptr) {
                neighbors.push_back(this->cells[i][j].get_north());
            }

            if(this->cells[i][j].get_east() != nullptr) {
                neighbors.push_back(this->cells[i][j].get_east());
            }

            if(neighbors.size() == 0) {
                continue;
            }

            this->cells[i][j].link(neighbors[this->randint(0, neighbors.size() - 1)]);
        }
    }
}

void Maze::print() const {
    std::cout << "+";
    for(unsigned int i=0; i<this->width; i++) {
        std::cout << "---+";
    }
    std::cout << std::endl;

    for(int i=this->height-1; i>=0; i--) {
        std::string top = "|";
        std::string bottom = "+";

        for(unsigned int j=0; j<this->width; j++) {
            const Cell* cell = &this->cells[i][j];

            std::string body = "   ";
            std::string east_boundary = cell->is_linked(cell->get_east()) ? " " : "|";

            top += body + east_boundary;

            std::string south_boundary = cell->is_linked(cell->get_south()) ? "   " : "---";
            std::string corner = "+";

            bottom += south_boundary + corner;
        }

        std::cout << top << std::endl;
        std::cout << bottom << std::endl;
    }
}

QByteArray Maze::create_image(unsigned int cell_size) const {
    unsigned int img_width = this->width * cell_size;
    unsigned int img_height = this->height * cell_size;

    QByteArray data(255, img_width * img_height);

    for(int i=this->height-1; i>=0; i--) {
        for(unsigned int j=0; j<this->width; j++) {
            const Cell* cell = &this->cells[i][j];

            unsigned int x1 = j * cell_size;
            unsigned int y1 = i * cell_size;

            unsigned int x2 = (j+1) * cell_size;
            unsigned int y2 = (i+1) * cell_size;

            if(cell->is_linked(cell->get_north())) {
                for(unsigned int x=x1; x <= x2; x++) {
                    data[y1 * this->width + x] = 0;
                }
            }
        }
    }

    return data;
}

/**
 * @brief      Configure the neighbors of each cell
 */
void Maze::configure_cells() {
    for(unsigned int i=0; i<this->height; i++) {
        for(unsigned int j=0; j<this->width; j++) {
            this->cells[i][j].set_neighbors(i < (this->height-1) ? &this->cells[i+1][j] : nullptr,
                                            i > 0 ? &this->cells[i-1][j] : nullptr,
                                            j < (this->width-1) ? &this->cells[i][j+1] : nullptr,
                                            j > 0 ? &this->cells[i][j-1] : nullptr);
        }
    }
}
