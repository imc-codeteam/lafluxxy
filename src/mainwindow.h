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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QLabel>
#include <QStatusBar>
#include <QTabWidget>
#include <QMenuBar>
#include <QMenu>

#include <iostream>

#include "inputtab.h"
#include "resultstab.h"
#include "two_dim_rd.h"
#include "worker_thread.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    QTabWidget *tabs;
    InputTab *input_tab;
    ResultsTab *results_tab;

    std::unique_ptr<TwoDimRD> tdrd;

public:
    explicit MainWindow(QWidget *parent = 0);

    ~MainWindow();

private slots:
    /**
     * @brief      Close the application
     */
    void exit();

    /**
     * @brief      Launch an RD simulation
     */
    void launch_calculation();

    /**
     * @brief      Handle results when the simulation is finished
     */
    void handle_simulation_finished();

    /**
     * @brief      Handle results when the simulation is canceled
     */
    void handle_simulation_canceled();

    /**
     * @brief      Handle the results of a single frame
     *
     * @param[in]  i      Frame index i
     * @param[in]  tcalc  Number of seconds spent on step
     */
    void handle_results_step(unsigned int i, double tcalc);

private:
    /**
     * @brief      Create tabs
     */
    void create_tabs();

    /**
     * @brief      Build the drop-down menus
     */
    void build_menu();
};

#endif // MAINWINDOW_H
