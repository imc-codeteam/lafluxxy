/**************************************************************************
 *   This file is part of LaFluxxy                                        *
 *   https://github.com/imc-codeteam/lafluxxy                             *
 *                                                                        *
 *   Author: Ivo Filot <i.a.w.filot@tue.nl>                               *
 *                                                                        *
 *   Bravais is free software: you can redistribute it and/or modify      *
 *   it under the terms of the GNU General Public License as published    *
 *   by the Free Software Foundation, either version 3 of the License,    *
 *   or (at your option) any later version.                               *
 *                                                                        *
 *   Bravais is distributed in the hope that it will be useful,           *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty          *
 *   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.              *
 *   See the GNU General Public License for more details.                 *
 *                                                                        *
 *   You should have received a copy of the GNU General Public License    *
 *   along with this program.  If not, see http://www.gnu.org/licenses/.  *
 *                                                                        *
 **************************************************************************/

#include "inputtab.h"

/**
 * @brief Input tab constructor
 * @param parent widget
 */
InputLotkaVolterra::InputLotkaVolterra(QWidget *parent) : QWidget(parent) {
    this->layout = new QVBoxLayout();
    this->setLayout(this->layout);

    // set label
    layout->addWidget(new QLabel(tr("<i>Lotka Volterra kinetic parameters</i>")));

    // build input boxes
    QWidget *widget = new QWidget();
    layout->addWidget(widget);
    this->gridlayout = new QGridLayout();
    widget->setLayout(this->gridlayout);
    this->build_input_boxes();
}

void InputLotkaVolterra::build_input_boxes() {
    for(unsigned int i=0; i<input_names.size(); i++) {
        this->gridlayout->addWidget(new QLabel(tr("<html>") + tr(input_labels[i].c_str()) + tr("</html>")), i, 0);
        QDoubleSpinBox *box = new QDoubleSpinBox();
        this->input_boxes.emplace(input_names[i], box);
        box->setValue(this->input_default_values[i]);
        this->gridlayout->addWidget(box, i, 1);
    }
}
