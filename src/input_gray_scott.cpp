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

#include "input_gray_scott.h"

/**
 * @brief Input tab constructor
 * @param parent widget
 */
InputGrayScott::InputGrayScott(QWidget *parent) : InputReaction(parent) {
    this->reaction_type = KINETICS::GRAY_SCOTT;
    this->input_names = {"f", "k"};
    this->input_labels = {"f", "k"};
    this->input_default_values = {0.029500, 0.056100};

    this->set_label();
    this->build_input_boxes();
    this->build_default_sets();
}

void InputGrayScott::set_label() {
    this->reaction_label->setText(tr("<i>Gray-Scott kinetic parameters</i>"));
}

/**
 * @brief      Gets the default parameter settings.
 *
 * @return     The default parameter settings.
 */
std::string InputGrayScott::get_default_parameter_settings() {
    // obtain default kinetic parameters
    unsigned int set_idx = this->combobox_default_sets->currentIndex();
    if(set_idx > 0 && set_idx <= this->kinetic_values_sets.size()) {
        this->input_default_values = {this->kinetic_values_sets[set_idx-1][0], this->kinetic_values_sets[set_idx-1][1]};
        this->build_input_boxes();
    }

    return std::string("dX=0.16;dY=0.08;dx=1.0;dt=0.5;width=256;height=256;steps=20;tsteps=1000;pbc=1");
}

/**
 * @brief      Add dropdown menu with default integration settings
 */
void InputGrayScott::build_default_sets() {
    this->combobox_default_sets = new QComboBox();

    this->combobox_default_sets->addItem(tr("Please select a default set..."));
    #ifdef _WIN32
    this->combobox_default_sets->addItem(QString::fromWCharArray(L"Pearson alpha pattern"));
    this->combobox_default_sets->addItem(QString::fromWCharArray(L"Pearson beta pattern"));
    this->combobox_default_sets->addItem(QString::fromWCharArray(L"Pearson gamma pattern"));
    this->combobox_default_sets->addItem(QString::fromWCharArray(L"Pearson delta pattern"));
    this->combobox_default_sets->addItem(QString::fromWCharArray(L"Pearson epsilon pattern"));
    this->combobox_default_sets->addItem(QString::fromWCharArray(L"Pearson zeta pattern"));
    this->combobox_default_sets->addItem(QString::fromWCharArray(L"Pearson eta pattern"));
    this->combobox_default_sets->addItem(QString::fromWCharArray(L"Pearson theta pattern"));
    this->combobox_default_sets->addItem(QString::fromWCharArray(L"Pearson iota pattern"));
    this->combobox_default_sets->addItem(QString::fromWCharArray(L"Pearson kappa pattern"));
    this->combobox_default_sets->addItem(QString::fromWCharArray(L"Pearson lambda pattern"));
    this->combobox_default_sets->addItem(QString::fromWCharArray(L"Pearson mu pattern"));
    #else
    this->combobox_default_sets->addItem(QString::fromWCharArray(L"Pearson α pattern"));
    this->combobox_default_sets->addItem(QString::fromWCharArray(L"Pearson β pattern"));
    this->combobox_default_sets->addItem(QString::fromWCharArray(L"Pearson γ pattern"));
    this->combobox_default_sets->addItem(QString::fromWCharArray(L"Pearson δ pattern"));
    this->combobox_default_sets->addItem(QString::fromWCharArray(L"Pearson ε pattern"));
    this->combobox_default_sets->addItem(QString::fromWCharArray(L"Pearson ζ pattern"));
    this->combobox_default_sets->addItem(QString::fromWCharArray(L"Pearson η pattern"));
    this->combobox_default_sets->addItem(QString::fromWCharArray(L"Pearson θ pattern"));
    this->combobox_default_sets->addItem(QString::fromWCharArray(L"Pearson ι pattern"));
    this->combobox_default_sets->addItem(QString::fromWCharArray(L"Pearson κ pattern"));
    this->combobox_default_sets->addItem(QString::fromWCharArray(L"Pearson λ pattern"));
    this->combobox_default_sets->addItem(QString::fromWCharArray(L"Pearson μ pattern"));
    #endif
    this->combobox_default_sets->setCurrentIndex(0);

    this->default_sets_gridlayout->addWidget(this->combobox_default_sets, 0, 0);
}
