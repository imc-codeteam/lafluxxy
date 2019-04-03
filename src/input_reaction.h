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
#include <unordered_map>

class InputReaction : public QWidget {
    Q_OBJECT

private:
    std::unordered_map<std::string, QDoubleSpinBox*> input_boxes;
    QGridLayout* gridlayout;

protected:
    QVBoxLayout* layout;
    QLabel* reaction_label;
    std::vector<std::string> input_names;
    std::vector<std::string> input_labels;
    std::vector<double> input_default_values;

public:
    /**
     * @brief Input tab constructor
     * @param parent widget
     */
    explicit InputReaction(QWidget *parent = 0);

protected:
    void build_input_boxes();

private:
    virtual void set_label() = 0;

private slots:

};

#endif // _INPUT_REACTION_H
