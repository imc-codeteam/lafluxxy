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
#include <QPushButton>
#include <QSpinBox>

#include "mazeholder.h"

class MazeTab : public QWidget {
    Q_OBJECT

private:
    QComboBox* maze_algo_selector;                  // select maze generation algorithm
    QPushButton* button_generate_mazes;             // button to construct mazes
    QVBoxLayout* layout;                            // main layout
    QSpinBox* input_maze_width;                     // width of the mazes
    QSpinBox* input_maze_height;                    // height of the mazes

    MazeHolder* mazeholder = nullptr;               // widget to hold generated mazes
    QPushButton* button_select_maze = nullptr;      // button to select the maze

    int maze_algo;                                  // type of maze generation algorithm

public:
    /**
     * @brief      Constructs the object.
     *
     * @param      parent  Parent Widget
     */
    explicit MazeTab(QWidget *parent = 0);

    inline QPushButton* get_button_select_maze() const {
        return this->button_select_maze;
    }

    /**
     * @brief      Get the selected maze
     *
     * @return     Pointer to maze.
     */
    Maze* get_maze() const;

private:

private slots:
    /**
     * @brief      Sets the maze generation algorithm type.
     *
     * @param[in]  maze_algo_type  The maze algorithm type
     */
    void set_algo_type(int maze_algo_type);

    /**
     * @brief      Builds mazes.
     */
    void build_mazes();

signals:
    void signal_mazes_generated();

};

#endif // _MAZETAB_H
