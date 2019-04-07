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

#include "mazetab.h"

/**
 * @brief      Constructs the object.
 *
 * @param      parent  Parent Widget
 */
MazeTab::MazeTab(QWidget *parent) : QWidget(parent) {
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

    this->layout = new QVBoxLayout;
    widget->setLayout(layout);

    // explanation label
    layout->addWidget(new QLabel(tr("Construct 2D mazes to introduce tortuosity in the simulation.")));

    // maze algo
    layout->addWidget(new QLabel(tr("<b>Maze generator algorithm</b>")));
    this->maze_algo_selector = new QComboBox();

    this->maze_algo_selector->addItem(tr("Please select an algorithm..."));
    this->maze_algo_selector->addItem(tr("Binary Tree"));
    this->maze_algo_selector->addItem(tr("Sidewinder"));
    this->maze_algo_selector->setCurrentIndex(0);
    layout->addWidget(this->maze_algo_selector);

    QWidget* mazedimholder = new QWidget;
    QGridLayout* mazedimgrid = new QGridLayout();
    mazedimholder->setLayout(mazedimgrid);
    this->input_maze_width = new QSpinBox();
    this->input_maze_width->setMinimum(4);
    this->input_maze_width->setMaximum(128);
    this->input_maze_width->setValue(16);
    this->input_maze_height = new QSpinBox();
    this->input_maze_height->setMinimum(4);
    this->input_maze_height->setMaximum(128);
    this->input_maze_height->setValue(16);
    mazedimgrid->addWidget(new QLabel(tr("Maze width")), 0, 0);
    mazedimgrid->addWidget(this->input_maze_width, 0, 1);
    mazedimgrid->addWidget(new QLabel(tr("Maze height")), 1, 0);
    mazedimgrid->addWidget(this->input_maze_height, 1, 1);
    this->layout->addWidget(mazedimholder);

    this->button_generate_mazes = new QPushButton("Generate mazes");
    this->button_generate_mazes->setEnabled(false);
    layout->addWidget(this->button_generate_mazes);

    // connect signals
    connect(this->maze_algo_selector, SIGNAL(currentIndexChanged(int)), SLOT(set_algo_type(int)));
    connect(this->button_generate_mazes, SIGNAL(released()), SLOT(build_mazes()));

    // maze results
    layout->addWidget(new QLabel(tr("<b>Maze results</b>")));
}

/**
 * @brief      Get the selected maze
 *
 * @return     Pointer to maze.
 */
Maze* MazeTab::get_maze() const {
    if(this->mazeholder != nullptr) {
        int maze_id = this->mazeholder->get_selected_maze_id();
        if(maze_id != -1) {
            return this->mazeholder->get_maze(maze_id * -1 - 2);
        }
    }

    return nullptr;
}

/**
 * @brief      Sets the maze generation algorithm type.
 *
 * @param[in]  maze_algo_type  The maze algorithm type
 */
void MazeTab::set_algo_type(int maze_algo_type) {
    this->maze_algo = maze_algo_type;
    if(maze_algo_type != 0) {
        this->button_generate_mazes->setEnabled(true);
    }
}

/**
 * @brief      Builds mazes.
 */
void MazeTab::build_mazes() {
    // clean up any old results
    if(this->mazeholder != nullptr) {
        delete this->mazeholder;
    }

    if(this->button_select_maze != nullptr) {
        delete this->button_select_maze;
    }

    this->mazeholder = new MazeHolder();
    this->layout->addWidget(this->mazeholder);
    this->mazeholder->build_mazes(this->input_maze_height->value(), this->input_maze_width->value());

    this->button_select_maze = new QPushButton("Select maze");
    this->layout->addWidget(this->button_select_maze);

    emit signal_mazes_generated();
}
