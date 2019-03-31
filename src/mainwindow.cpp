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

#include "mainwindow.h"
#include "config.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    QWidget *widget = new QWidget();
    this->setCentralWidget(widget);

    // create tabs
    this->create_tabs();

    // create vertical layout
    QVBoxLayout *layout = new QVBoxLayout();
    layout->setMargin(5);
    layout->addWidget(this->tabs);
    widget->setLayout(layout);

    // display status message
    statusBar()->showMessage(tr("Welcome to ") + tr(PROGRAM_NAME));

    // set window parameters
    this->setWindowTitle(tr(PROGRAM_NAME) + tr(" v.") + QString(PROGRAM_VERSION));    // set window label
    this->setMinimumSize(1024, 768);                     // set minimum size of the window
    this->resize(1024, 768);                             // resize the windows

    // connect signals
}

void MainWindow::create_tabs() {
    this->tabs = new QTabWidget();
    this->input_tab = new InputTab();
    this->tabs->addTab(this->input_tab, tr("Input"));
}

MainWindow::~MainWindow(){}
