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

#pragma once

#include <memory>
#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QButtonGroup>
#include <QRadioButton>

#include "mazerenderer.h"
#include "vendor/mazebuilder/src/maze_builder.h"

class MazeHolder : public QWidget {
    Q_OBJECT

private:
    QGridLayout* mazegrid;                          // grid to store mazes in
    std::vector<std::unique_ptr<Maze> > mazes;      // store mazes

    MazeRenderer mz;                                // object to render images from a maze
    MazeBuilder mb;                                 // object that build mazes

    QButtonGroup* maze_selector;                    // select maze

public:
    /**
     * @brief      Constructs the object.
     *
     * @param      parent  Parent Widget
     */
    explicit MazeHolder(QWidget *parent = 0);

    /**
     * @brief      Builds mazes.
     *
     * @param[in]  mrows  Number of rows in mazes
     * @param[in]  mcols  Number of columns in mazes
     */
    void build_mazes(unsigned int mrows, unsigned int mcols);

    inline int get_selected_maze_id() const {
        return this->maze_selector->checkedId();
    }

    inline Maze* get_maze(unsigned int id) const {
        return this->mazes[id].get();
    }

private:
    /**
     * @brief      Generate single maze
     *
     * @param      mazegrid  Target layout to place Maze image in
     * @param[in]  rows      Number of rows for the mazes
     * @param[in]  cols      Number of columns for the mazes
     * @param[in]  row       Row number in grid
     * @param[in]  col       Column number in grid
     */
    void generate_maze(QGridLayout* mazegrid, unsigned int rows = 10, unsigned int cols = 10, unsigned int row = 0, unsigned int col = 0);

private slots:

};
