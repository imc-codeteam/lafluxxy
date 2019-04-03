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

#ifndef INPUTTAB_H
#define INPUTTAB_H

#include <QWidget>
#include <QComboBox>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>
#include <QDoubleSpinBox>

#include "two_dim_rd.h"
#include "input_lotka_volterra.h"
#include "input_gray_scott.h"
#include "reaction_lotka_volterra.h"

class InputTab : public QWidget {
    Q_OBJECT

private:
    QComboBox* reaction_selector;       // set reaction type
    QWidget* reaction_settings;         // widget that holds reaction settings

    QDoubleSpinBox* input_diffusion_X;  // set diffusion of component X
    QDoubleSpinBox* input_diffusion_Y;  // set diffusion of component Y
    QDoubleSpinBox* input_dx;           // set distance interval
    QDoubleSpinBox* input_dt;           // set time interval

    QSpinBox* input_width;              // set width of box
    QSpinBox* input_height;             // set height of box
    QSpinBox* input_steps;              // set number of integration steps
    QSpinBox* input_tsteps;             // set number of time steps for each integration step

    QGridLayout* gridlayout_reaction;
    QPushButton* button_submit;

public:
    /**
     * @brief Input tab constructor
     * @param parent widget
     */
    explicit InputTab(QWidget *parent = 0);

    inline QPushButton* get_button_submit() const {
        return this->button_submit;
    }

    TwoDimRD* build_reaction_system();

private:
    /**
     * @brief      Build selector widget for reactions
     *
     * @return     Selector widget
     */
    void build_reaction_settings(QGridLayout *gridlayout);

    /**
     * @brief      Builds general parameters.
     */
    void build_general_parameters(QGridLayout *gridlayout);

private slots:
    /**
     * @brief      Sets the reaction input widget
     *
     * @param[in]  reactype  The type
     */
    void set_reaction_input(int reactype);
};

#endif // INPUTTAB_H
