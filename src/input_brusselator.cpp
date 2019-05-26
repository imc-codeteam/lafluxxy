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

#include "input_brusselator.h"

/**
 * @brief Input tab constructor
 * @param parent widget
 */
InputBrusselator::InputBrusselator(QWidget *parent) : InputReaction(parent) {
    this->reaction_type = KINETICS::BRUSSELATOR;
    this->input_names = {"alpha", "beta"};
    this->input_labels = {"&alpha;", "&beta;"};
    this->input_default_values = {4.50, 7.50};

    this->set_label();
    this->build_input_boxes();
    this->build_default_sets();
}

void InputBrusselator::set_label() {
    this->reaction_label->setText(tr("<i>Brusselator kinetic parameters</i>"));
    this->show_reaction_equation("reaction_equation_brusselator.png");
}

/**
 * @brief      Gets the default parameter settings.
 *
 * @return     The default parameter settings.
 */
std::string InputBrusselator::get_default_parameter_settings() {
    // obtain default kinetic parameters
    unsigned int set_idx = this->combobox_default_sets->currentIndex();
    if(set_idx > 0 && set_idx <= this->kinetic_values_sets.size()) {
        this->input_default_values = {this->kinetic_values_sets[set_idx-1][0], this->kinetic_values_sets[set_idx-1][1]};
        this->build_input_boxes();
        return (boost::format("dX=%.1f;dY=10;dx=1.0;dt=0.005;width=256;height=256;steps=20;tsteps=1000;pbc=1") % this->kinetic_values_sets[set_idx-1][2]).str();
    } else {
        return std::string("dX=2;dY=16;dx=1.0;dt=0.005;width=256;height=256;steps=20;tsteps=1000;pbc=1");
    }
}

/**
 * @brief      Add dropdown menu with default integration settings
 */
void InputBrusselator::build_default_sets() {
    this->combobox_default_sets = new QComboBox();

    this->combobox_default_sets->addItem(tr("Please select a default set..."));
    this->combobox_default_sets->addItem(QString::fromWCharArray(L"Hexagons pattern"));
    this->combobox_default_sets->addItem(QString::fromWCharArray(L"Stripes pattern"));
    this->combobox_default_sets->addItem(QString::fromWCharArray(L"Transition pattern"));
    this->combobox_default_sets->setCurrentIndex(0);

    this->default_sets_gridlayout->addWidget(this->combobox_default_sets, 0, 0);
}
