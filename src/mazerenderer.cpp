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

#include "mazerenderer.h"

MazeRenderer::MazeRenderer() {

}

std::vector<uint8_t> MazeRenderer::create_image(const Maze& maze, unsigned int cell_size) {
    unsigned int img_width = maze.get_width() * cell_size + 1;
    unsigned int img_height = maze.get_height() * cell_size + 1;
    img_width += (4 - img_width % 4);
    img_height += (4 - img_height % 4);

    std::cout << img_width << "x" << img_height << std::endl;

    std::vector<uint8_t> data(img_width * img_height, 255);

    const auto& cells = maze.get_cells();

    for(unsigned int i=0; i<maze.get_height(); i++) {
        for(unsigned int j=0; j<maze.get_width(); j++) {
            const Cell* cell = &cells[i][j];

            unsigned int x1 = j * cell_size;
            unsigned int y1 = img_height - ((i+1) * cell_size) - 1;

            unsigned int x2 = (j+1) * cell_size;
            unsigned int y2 = img_height - (i * cell_size) - 1;

            if(cell->get_north() == nullptr) {
                this->draw_line(data, img_width, x1, y1, x2, y1);
            }

            if(cell->get_west() == nullptr) {
                this->draw_line(data, img_width, x1, y1, x1, y2);
            }

            if(!cell->is_linked(cell->get_east())) {
                this->draw_line(data, img_width, x2, y1, x2, y2);
            }

            if(!cell->is_linked(cell->get_south())) {
                this->draw_line(data, img_width, x1, y2, x2, y2);
            }
        }
    }

    return data;
}

void MazeRenderer::draw_line(std::vector<uint8_t>& data, unsigned int width, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2) {
    if(x1 == x2) {
        for(unsigned int y=y1; y<y2; y++) {
            data[y * width + x1] = 0;
        }
        return;
    }

    if(y1 == y2) {
        for(unsigned int x=x1; x<x2; x++) {
            data[y1 * width + x] = 0;
        }

        return;
    }
}
