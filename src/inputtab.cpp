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

#include "inputtab.h"

/**
 * @brief Input tab constructor
 * @param parent widget
 */
InputTab::InputTab(QWidget *parent) : QWidget(parent), reaction_settings(nullptr) {
    QVBoxLayout *mainLayout = new QVBoxLayout;
    this->setLayout(mainLayout);
    this->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

    // add a ScrollArea widget and define properties
    QScrollArea *scrollArea = new QScrollArea(this);     //Create scroll area Widget
    scrollArea->setContentsMargins(0,0,0,0);
    scrollArea->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    scrollArea->setWidgetResizable(true);

    // add ScrollArea to QWidget
    mainLayout->addWidget(scrollArea);

    // create new Widget for in the QScrollArea and set properties
    QWidget* widget = new QWidget();
    widget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    // add Widget to ScrollArea
    scrollArea->setWidget(widget);

    QVBoxLayout *layout = new QVBoxLayout;
    widget->setLayout(layout);

    // add reaction settings
    QWidget* gridwidget_reaction = new QWidget();
    layout->addWidget(gridwidget_reaction);
    gridwidget_reaction->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    this->gridlayout_reaction = new QGridLayout();
    gridwidget_reaction->setLayout(this->gridlayout_reaction);
    this->gridlayout_reaction->setColumnStretch(1, 1);
    this->build_reaction_settings(this->gridlayout_reaction);

    // build general parameters
    QWidget* gridwidget = new QWidget();
    layout->addWidget(gridwidget);
    gridwidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    QGridLayout *gridlayout = new QGridLayout();
    gridwidget->setLayout(gridlayout);
    gridlayout->setColumnStretch(1, 1);
    this->build_general_parameters(gridlayout);

    // connect signals
    connect(this->reaction_selector, SIGNAL(currentIndexChanged(int)), SLOT(set_reaction_input(int)));
}

/**
 * @brief      Build selector widget for reactions
 */
void InputTab::build_reaction_settings(QGridLayout *gridlayout) {
    this->reaction_selector = new QComboBox();

    this->reaction_selector->addItem(tr("Please select a reaction..."));
    this->reaction_selector->addItem(tr("Lotka-Volterra"));
    this->reaction_selector->addItem(tr("Gray-Scott"));
    this->reaction_selector->addItem(tr("Fitzhugh-Nagumo"));
    this->reaction_selector->addItem(tr("Brusselator"));
    this->reaction_selector->addItem(tr("Barkley"));
    this->reaction_selector->setCurrentIndex(0);

    gridlayout->addWidget(new QLabel(tr("<b>Reaction settings</b>")), 0, 0);
    gridlayout->addWidget(this->reaction_selector, 1, 0);
}

/**
 * @brief      Builds general parameters.
 */
void InputTab::build_general_parameters(QGridLayout *gridlayout) {
    this->input_diffusion_X = new QDoubleSpinBox();
    this->input_diffusion_Y = new QDoubleSpinBox();
    this->input_dx = new QDoubleSpinBox();
    this->input_dt = new QDoubleSpinBox();

    unsigned int row = 0;

    gridlayout->addWidget(new QLabel("<b>Integration settings</b>"), row, 0);
    row++;

    gridlayout->addWidget(new QLabel("dX"), row, 0);
    gridlayout->addWidget(this->input_diffusion_X, row, 1);
    gridlayout->addWidget(new QLabel("Diffusion rate of component X"), row, 2);
    row++;

    gridlayout->addWidget(new QLabel("dY"), row, 0);
    gridlayout->addWidget(this->input_diffusion_Y, row, 1);
    gridlayout->addWidget(new QLabel("Diffusion rate of component Y"), row, 2);
    row++;

    gridlayout->addWidget(new QLabel("dx"), row, 0);
    gridlayout->addWidget(this->input_dx, row, 1);
    gridlayout->addWidget(new QLabel("Spatial distance in discretization"), row, 2);
    row++;

    gridlayout->addWidget(new QLabel("dt"), row, 0);
    gridlayout->addWidget(this->input_dt, row, 1);
    gridlayout->addWidget(new QLabel("Time step size"), row, 2);
    row++;

    this->input_width = new QSpinBox();
    this->input_height = new QSpinBox();
    this->input_steps = new QSpinBox();
    this->input_tsteps = new QSpinBox();

    gridlayout->addWidget(new QLabel("width"), row, 0);
    gridlayout->addWidget(this->input_width, row, 1);
    gridlayout->addWidget(new QLabel("Width of the unit cell"), row, 2);
    row++;

    gridlayout->addWidget(new QLabel("height"), row, 0);
    gridlayout->addWidget(this->input_height, row, 1);
    gridlayout->addWidget(new QLabel("Height of the unit cell"), row, 2);
    row++;

    gridlayout->addWidget(new QLabel("steps"), row, 0);
    gridlayout->addWidget(this->input_steps, row, 1);
    gridlayout->addWidget(new QLabel("Number of integration steps"), row, 2);
    row++;

    gridlayout->addWidget(new QLabel("tsteps"), row, 0);
    gridlayout->addWidget(this->input_tsteps, row, 1);
    gridlayout->addWidget(new QLabel("Number of time steps per each integration step"), row, 2);
    row++;
}

/**
 * @brief      Sets the reaction input widget
 *
 * @param[in]  reactype  The type
 */
void InputTab::set_reaction_input(int reactype) {
    if(this->reaction_settings != nullptr) {
        this->gridlayout_reaction->removeWidget(this->reaction_settings);
        delete this->reaction_settings;
    }

    switch(reactype) {
        case 1:
            this->reaction_settings = new InputLotkaVolterra();
        break;
        case 2:
            this->reaction_settings = new InputGrayScott();
        break;
        default:
            // do nothing
        break;
    }

    if(this->reaction_settings != nullptr) {
        this->gridlayout_reaction->addWidget(this->reaction_settings, 2, 0);
    }
}
