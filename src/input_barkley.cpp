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

#include "input_barkley.h"

/**
 * @brief Input tab constructor
 * @param parent widget
 */
InputBarkley::InputBarkley(QWidget *parent) : InputReaction(parent) {
    this->reaction_type = KINETICS::BARKLEY;
    this->input_names = {"alpha", "beta", "epsilon"};
    this->input_labels = {"&alpha;", "&beta;", "&epsilon;"};
    this->input_default_values = {0.75, 0.06, 50.0};

    this->set_label();
    this->build_input_boxes();
}

void InputBarkley::set_label() {
    this->reaction_label->setText(tr("<i>Barkley kinetic parameters</i>"));
}

/**
 * @brief      Gets the default parameter settings.
 *
 * @return     The default parameter settings.
 */
std::string InputBarkley::get_default_parameter_settings() {
    return std::string("dX=5.0;dY=0.0;dx=1.0;dt=0.001;width=128;height=128;steps=20;tsteps=1000;pbc=0");
}
