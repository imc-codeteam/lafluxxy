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

#include "input_lotka_volterra.h"

/**
 * @brief Input tab constructor
 * @param parent widget
 */
InputLotkaVolterra::InputLotkaVolterra(QWidget *parent) : InputReaction(parent) {
    this->input_names = {"alpha", "beta", "gamma", "delta"};
    this->input_labels = {"&alpha;", "&beta;", "&gamma;", "&delta;"};
    this->input_default_values = {2.3333, 2.6666, 1.0, 1.0};

    this->set_label();
    this->build_input_boxes();
}

void InputLotkaVolterra::set_label() {
    this->reaction_label->setText(tr("<i>Lotka-Volterra kinetic parameters</i>"));
}
