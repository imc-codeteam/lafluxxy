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

#ifndef _MAZE_RENDERER_H
#define _MAZE_RENDERER_H

#include <QPixmap>
#include <array>
#include <memory>

#include "maze.h"
#include "colorscheme.h"

class MazeRenderer {
private:
    std::unique_ptr<ColorScheme> color_scheme;

public:
    MazeRenderer();

    std::vector<uint8_t> create_image(const Maze& maze, unsigned int cell_size = 10);

    /**
     * @brief      Sets the color scheme.
     *
     * @param[in]  name  The name
     */
    inline void set_color_scheme(const std::string& name) {
        this->color_scheme = std::make_unique<ColorScheme>(name);
    }

    QPixmap generate_maze_pixmap(const Maze& maze, unsigned int maxsize);

private:
    void draw_line(std::vector<uint8_t>& data, unsigned int width, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, const std::array<uint8_t, 3>& color);

    void fill_rectangle(std::vector<uint8_t>& data, unsigned int width, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, const std::array<uint8_t, 3>& color);

    /**
     * @brief      Obtain color from data point using color scheme
     *
     * @param[in]  val     The value
     * @param[in]  minval  Minimum value
     * @param[in]  maxval  Maximum value
     *
     * @return     The color.
     */
    std::array<uint8_t, 3> get_color(double val, double minval, double maxval) const;
};

#endif // _MAZE_RENDERER_H
