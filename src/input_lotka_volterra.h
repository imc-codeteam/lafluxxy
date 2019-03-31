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

#ifndef _INPUT_LOTKA_VOLTERRA_H
#define _INPUT_LOTKA_VOLTERRA_H

#include <QWidget>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QDoubleSpinBox>
#include <unordered_map>

class InputLotkaVolterra : public QWidget {
    Q_OBJECT

private:
    const std::vector<std::string> input_names = {"alpha", "beta", "gamma", "delta"};
    const std::vector<std::string> input_labels = {"&alpha;", "&beta;", "&gamma;", "&delta;"};
    const std::vector<double> input_default_values = {2.3333, 2.6666, 1.0, 1.0};
    std::unordered_map<std::string, QDoubleSpinBox*> input_boxes;
    QVBoxLayout* layout;
    QGridLayout* gridlayout;

public:
    /**
     * @brief Input tab constructor
     * @param parent widget
     */
    explicit InputLotkaVolterra(QWidget *parent = 0);

private:
    void build_input_boxes();

private slots:

};

#endif // _INPUT_LOTKA_VOLTERRA_H
