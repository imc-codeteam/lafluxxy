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

#ifndef _INPUT_REACTION_H
#define _INPUT_REACTION_H

#include <QWidget>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QStyle>
#include <QComboBox>
#include <unordered_map>

#include "config.h"

class InputReaction : public QWidget {
    Q_OBJECT

private:
    std::unordered_map<std::string, QDoubleSpinBox*> input_boxes;
    QGridLayout *kinetic_param_gridlayout;
    QPushButton *button_set_defaults;

protected:
    unsigned int reaction_type;

    QVBoxLayout* layout;
    QLabel* reaction_label;
    QGridLayout *default_sets_gridlayout;
    std::vector<std::string> input_names;
    std::vector<std::string> input_labels;
    std::vector<double> input_default_values;

public:
    /**
     * @brief Input tab constructor
     * @param parent widget
     */
    explicit InputReaction(QWidget *parent = 0);

    /**
     * @brief      Gets the reaction type.
     *
     * @return     The reaction type.
     */
    inline unsigned int get_reaction_type() const {
        return this->reaction_type;
    }

    /**
     * @brief      Gets the parameter string that defines the kinetic parameters.
     *
     * @return     The parameter string.
     */
    std::string get_parameter_string() const;

    /**
     * @brief      Gets the default parameter settings.
     *
     * @return     The default parameter settings.
     */
    virtual std::string get_default_parameter_settings() = 0;

    /**
     * @brief      Gets the pointer to button for setting defaults
     *
     * @return     The button
     */
    inline QPushButton* get_button_set_defaults() const {
        return this->button_set_defaults;
    }

protected:
    /**
     * @brief      Builds input boxes.
     */
    void build_input_boxes();

private:
    /**
     * @brief      Sets the label.
     */
    virtual void set_label() = 0;

private slots:

};

#endif // _INPUT_REACTION_H
