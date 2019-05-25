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

#include "reaction_brusselator.h"

ReactionBrusselator::ReactionBrusselator() {
    this->reacttype = KINETICS::BRUSSELATOR;
}

void ReactionBrusselator::init(MatrixXXd& a, MatrixXXd& b) const {
    this->init_random(a, b, this->alpha, this->beta / this->alpha, 0.3);
}

void ReactionBrusselator::reaction(double a, double b, double *ra, double *rb) const {
    *ra = this->alpha - (this->beta + 1.0) * a + (a * a * b);
    *rb = (this->beta * a) - (a * a * b);
}

/**
 * @brief      Sets the parameters.
 *
 * @param[in]  params  The parameters
 */
void ReactionBrusselator::set_parameters(const std::string& params) {
    auto map = this->parse_parameters(params);

    auto got = map.find("alpha");
    if(got != map.end()) {
        this->alpha = got->second;
    } else {
        throw std::runtime_error("Cannot find parameter alpha");
    }

    got = map.find("beta");
    if(got != map.end()) {
        this->beta = got->second;
    } else {
        throw std::runtime_error("Cannot find parameter beta");
    }

    // std::vector<std::string> paramlist = {"alpha", "beta"};
    // std::cout << "Succesfully loaded the following parameters" << std::endl;
    // for(const std::string& variable : paramlist) {
    //     auto got = map.find(variable);
    //     std::cout << "    " << variable << " = " << got->second << std::endl;
    // }
}

/**
 * @brief      Gets the kinetic parameters.
 *
 * @return     The kinetic parameters.
 */
std::array<double, 4> ReactionBrusselator::get_kinetic_parameters() const {
    return {this->alpha, this->beta, 0.0, 0.0};
}
