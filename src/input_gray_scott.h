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

#ifndef _INPUT_GRAY_SCOTT
#define _INPUT_GRAY_SCOTT

#include <QWidget>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QDoubleSpinBox>
#include <unordered_map>

#include <array>

#include "input_reaction.h"

class InputGrayScott : public InputReaction {

private:
    QComboBox* combobox_default_sets;

    // set default kinetic parameters
    const std::vector<std::array<double, 2> > kinetic_values_sets = {
        {0.0175, 0.0504},   // alpha
        {0.0180, 0.0460},   // beta
        {0.0250, 0.0560},   // gamma
        {0.0300, 0.0560},   // delta
        {0.0208, 0.0576},   // epsilon
        {0.0250, 0.0060},   // zeta
        {0.0404, 0.0638},   // eta
        {0.0295, 0.0561},   // theta
        {0.0500, 0.0600},   // iota
        {0.0400, 0.0620},   // kappa
        {0.0392, 0.0649},   // lambda
        {0.0416, 0.0625}    // mu

    };

public:
    /**
     * @brief Input tab constructor
     * @param parent widget
     */
    explicit InputGrayScott(QWidget *parent = 0);

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

#endif // _INPUT_GRAY_SCOTT
