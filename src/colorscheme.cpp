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

#include "colorscheme.h"

ColorScheme::ColorScheme(const std::string& name) {
    if(name == "magma") {
        this->colors = magma;
        return;
    }

    if(name == "viridis") {
        this->colors = viridis;
        return;
    }

    throw std::runtime_error("Unknown color scheme \"" + name + "\" requested.");
}

/**
 * @brief      Obtain color from data point using color scheme
 *
 * @param[in]  val     The value
 * @param[in]  minval  Minimum value
 * @param[in]  maxval  Maximum value
 *
 * @return     The color.
 */
std::array<uint8_t, 3> ColorScheme::get_color(double val, double minval, double maxval) const {
    if(val <= minval) {
        return std::array<uint8_t, 3>{uint8_t(this->colors[0] * 256.0f), uint8_t(this->colors[1] * 256.0f), uint8_t(this->colors[2] * 256.0f)};
    }

    if(val >= maxval) {
        const unsigned int sz = this->colors.size();
        return std::array<uint8_t, 3>{uint8_t(this->colors[sz-3] * 256.0f), uint8_t(this->colors[sz-2] * 256.0f), uint8_t(this->colors[sz-1] * 256.0f)};
    }

    unsigned int idx = (val - minval) / (maxval - minval) * (this->colors.size() / 3 - 1);

    return std::array<uint8_t, 3>{uint8_t(this->colors[idx*3] * 256.0f), uint8_t(this->colors[idx*3+1] * 256.0f), uint8_t(this->colors[idx*3+2] * 256.0f)};
}
