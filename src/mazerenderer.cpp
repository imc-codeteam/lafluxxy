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
    this->set_color_scheme("viridis");
}

std::vector<uint8_t> MazeRenderer::create_image(const Maze& maze, unsigned int cell_size) {
    unsigned int img_width = maze.cols() * cell_size + 1;
    unsigned int img_height = maze.rows() * cell_size + 1;
    img_width += (img_width * 3) % 4;
    img_height += (img_height * 3) % 4;

    // perform Dijkstra algorithm
    auto dijkstra = this->mazestats.perform_dijkstra(maze, maze.cols()/2, maze.rows()/2);

    std::vector<uint8_t> data(img_width * img_height * 3, 255);
    const double minval = 0.0;
    const double maxval = (double)dijkstra.maxCoeff();

    // fill background
    for(unsigned int i=0; i<maze.rows(); i++) {
        for(unsigned int j=0; j<maze.cols(); j++) {
            unsigned int x1 = j * cell_size;
            unsigned int y1 = img_height - ((i+1) * cell_size) - 1;

            unsigned int x2 = (j+1) * cell_size;
            unsigned int y2 = img_height - (i * cell_size) - 1;

            auto col = this->color_scheme->get_color((double)(maxval - dijkstra(i,j)), minval, maxval);
            this->fill_rectangle(data, img_width, x1, y1, x2, y2, col);
        }
    }

    // draw boundaries
    for(unsigned int i=0; i<maze.rows(); i++) {
        for(unsigned int j=0; j<maze.cols(); j++) {
            unsigned int x1 = j * cell_size;
            unsigned int y1 = img_height - ((i+1) * cell_size) - 1;

            unsigned int x2 = (j+1) * cell_size;
            unsigned int y2 = img_height - (i * cell_size) - 1;

            // draw cell borders
            if(maze(i,j).get_north() == nullptr) {
                this->draw_line(data, img_width, x1, y1, x2, y1, std::array<uint8_t, 3>{0, 0, 0});
            }

            if(maze(i,j).get_west() == nullptr) {
                this->draw_line(data, img_width, x1, y1, x1, y2, std::array<uint8_t, 3>{0, 0, 0});
            }

            if(!maze(i,j).is_linked(maze(i,j).get_east())) {
                this->draw_line(data, img_width, x2, y1, x2, y2, std::array<uint8_t, 3>{0, 0, 0});
            }

            if(!maze(i,j).is_linked(maze(i,j).get_south())) {
                this->draw_line(data, img_width, x1, y2, x2, y2, std::array<uint8_t, 3>{0, 0, 0});
            }
        }
    }

    return data;
}

/**
 * @brief      Generate a pixmap from a maze
 *
 * @param[in]  maze     The maze
 * @param[in]  maxsize  Maximum size of img in px
 *
 * @return     The maze pixmap.
 */
QPixmap MazeRenderer::generate_maze_pixmap(const Maze& maze, unsigned int maxsize) {
    unsigned int cell_size = maxsize / std::max(maze.cols(), maze.rows());
    std::vector<uint8_t> graph_data = this->create_image(maze, cell_size);

    unsigned int img_width = maze.cols() * cell_size + 1;
    unsigned int img_height = maze.rows() * cell_size + 1;
    unsigned int mazewidth = img_width;
    unsigned int mazeheight = img_height;

    // Qt Images need to be 32-bits aligned
    img_width += ((img_width * 3) % 4);
    img_height += ((img_height * 3) % 4);

    QImage img(&graph_data[0], img_width, img_height, QImage::Format_RGB888);
    QImage cropped = img.copy(0, img_height - mazeheight, mazewidth, mazeheight);
    QPixmap pixmap = QPixmap::fromImage(cropped);

    return pixmap;
}

void MazeRenderer::draw_line(std::vector<uint8_t>& data, unsigned int width, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, const std::array<uint8_t, 3>& color) {
    if(x1 == x2) {
        for(unsigned int y=y1; y<y2; y++) {
            const unsigned int idx = (y * width + x1) * 3;
            data[idx] = color[0];
            data[idx+1] = color[1];
            data[idx+2] = color[2];
        }
        return;
    }

    if(y1 == y2) {
        for(unsigned int x=x1; x<x2; x++) {
            const unsigned int idx = (y1 * width + x) * 3;
            data[idx] = color[0];
            data[idx+1] = color[1];
            data[idx+2] = color[2];
        }

        return;
    }
}

void MazeRenderer::fill_rectangle(std::vector<uint8_t>& data, unsigned int width, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, const std::array<uint8_t, 3>& color) {
    for(unsigned int y=y1; y<y2; y++) {
        for(unsigned int x=x1; x<x2; x++) {
            const unsigned int idx = (y * width + x) * 3;
            data[idx] = color[0];
            data[idx+1] = color[1];
            data[idx+2] = color[2];
        }
    }
}
