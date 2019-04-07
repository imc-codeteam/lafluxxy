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

#ifndef _MAZETAB_H
#define _MAZETAB_H

#include <QWidget>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QLabel>
#include <QPainter>
#include <QComboBox>

#include "mazerenderer.h"

class MazeTab : public QWidget {
    Q_OBJECT

private:
    QComboBox* maze_algo_selector;

public:
    /**
     * @brief Maze tab constructor
     * @param parent widget
     */
    explicit MazeTab(QWidget *parent = 0);


private:
    void generate_maze(QGridLayout* mazegrid, unsigned int grow, unsigned int gcol);

private slots:

};

#endif // _MAZETAB_H
