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

#ifndef _INPUT_BRUSSELATOR_H
#define _INPUT_BRUSSELATOR_H

#include <QWidget>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QDoubleSpinBox>
#include <unordered_map>
#include <iostream>
#include <array>

#include <boost/format.hpp>

#include "config.h"
#include "input_reaction.h"

class InputBrusselator : public InputReaction {

private:
    QComboBox* combobox_default_sets;

    // set default kinetic parameters
    const std::vector<std::array<double, 3> > kinetic_values_sets = {
        {3.0, 10.5, 5.5},   // hexagons
        {3.0, 8.5, 3.0},    // stripes
        {3.0, 10.0, 3.0}    // transition pattern
    };

public:
    /**
     * @brief Input tab constructor
     * @param parent widget
     */
    explicit InputBrusselator(QWidget *parent = 0);

    /**
     * @brief      Gets the default parameter settings.
     *
     * @return     The default parameter settings.
     */
    std::string get_default_parameter_settings() override;

private:
    void set_label() override;

    /**
     * @brief      Add dropdown menu with default integration settings
     */
    void build_default_sets();

private slots:

};

#endif // _INPUT_BRUSSELATOR_H
