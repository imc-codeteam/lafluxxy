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

#include "input_reaction.h"

InputReaction::InputReaction(QWidget *parent) : QWidget(parent) {
    this->layout = new QVBoxLayout();
    this->setLayout(this->layout);

    // set reaction label
    this->reaction_label = new QLabel();
    this->layout->addWidget(this->reaction_label);

    // build grid layout for the input boxes
    QWidget *kinetic_param_widget = new QWidget();
    layout->addWidget(kinetic_param_widget);
    this->kinetic_param_gridlayout = new QGridLayout();
    kinetic_param_widget->setLayout(this->kinetic_param_gridlayout);

    // create button to allow the user to pick default settings
    QWidget *default_settings_widget = new QWidget();
    layout->addWidget(default_settings_widget);
    this->default_sets_gridlayout = new QGridLayout();
    default_settings_widget->setLayout(this->default_sets_gridlayout);
    this->button_set_defaults = new QPushButton(" Set default integration settings");
    QIcon icon_button_set_defaults = style()->standardIcon(QStyle::SP_ArrowDown);
    this->button_set_defaults->setIcon(icon_button_set_defaults);
    this->button_set_defaults->setToolTip("Overwrite the integration settings below with default settings for this kinetic system.");
    this->default_sets_gridlayout->addWidget(this->button_set_defaults, 0, 1);
}

/**
 * @brief      Builds input boxes.
 */
void InputReaction::build_input_boxes() {
    // always clear any existing items in the input boxes
    this->input_boxes.clear();

    // build new boxes
    for(unsigned int i=0; i<input_names.size(); i++) {
        this->kinetic_param_gridlayout->addWidget(new QLabel(tr("<html>") + tr(input_labels[i].c_str()) + tr("</html>")), i, 0);
        QDoubleSpinBox *box = new QDoubleSpinBox();
        box->setDecimals(6);
        box->setMinimum(-100.0);
        box->setMaximum(100.0);
        this->input_boxes.emplace(input_names[i], box);
        box->setValue(this->input_default_values[i]);
        this->kinetic_param_gridlayout->addWidget(box, i, 1);
    }
}

/**
 * @brief      Gets the parameter string that defines the kinetic parameters.
 *
 * @return     The parameter string.
 */
std::string InputReaction::get_parameter_string() const {
    std::string parameter_string;

    for(auto it : this->input_boxes) {
        parameter_string += it.first;
        parameter_string += "=";
        parameter_string += std::to_string(it.second->value());
        parameter_string += ";";
    }

    parameter_string.pop_back();

    return parameter_string;
}
