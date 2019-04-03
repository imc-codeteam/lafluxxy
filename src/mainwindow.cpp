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
    //build drop-down menu
    this->build_menu();

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

    // build connections
    connect(this->input_tab->get_button_submit(), SIGNAL(clicked()), this, SLOT(launch_calculation()));
}

/**
 * @brief      Destroys the object.
 */
MainWindow::~MainWindow(){}

/**
 * @brief      Close the application
 */
void MainWindow::exit() {
    QApplication::quit();
}

/**
 * @brief      Launch the simulation
 */
void MainWindow::launch_calculation() {
    this->tdrd = std::unique_ptr<TwoDimRD>(this->input_tab->build_reaction_system());
    this->results_tab->set_reaction_system(this->tdrd.get());

    this->input_tab->get_button_submit()->setEnabled(false);

    // already set initial conditions image
    this->results_tab->update_progress(0, this->tdrd->get_num_steps());
    this->results_tab->add_frame(0);

    WorkerThread *workerThread = new WorkerThread(this->tdrd.get());
    connect(workerThread, &WorkerThread::simulation_finished, this, &MainWindow::handle_results);
    connect(workerThread, &WorkerThread::step_finished, this, &MainWindow::handle_results_step);
    connect(workerThread, &WorkerThread::finished, workerThread, &QObject::deleteLater);
    workerThread->start();

    this->tabs->setCurrentIndex(1);
    statusBar()->showMessage(tr("Simulation running..."));
}

/**
 * @brief      Handle results when the simulation is finished
 */
void MainWindow::handle_results() {
    this->input_tab->get_button_submit()->setEnabled(true);
    statusBar()->showMessage(tr("Simulation complete."));
}

/**
 * @brief      Handle the results of a single frame
 *
 * @param[in]  i     Frame index i
 */
void MainWindow::handle_results_step(unsigned int i) {
    this->results_tab->update_progress(i+1, this->tdrd->get_num_steps());
    this->results_tab->add_frame(i+1);
}

/**
 * @brief      Create tabs
 */
void MainWindow::create_tabs() {
    this->tabs = new QTabWidget();
    this->input_tab = new InputTab();
    this->tabs->addTab(this->input_tab, tr("Input"));

    this->results_tab = new ResultsTab();
    this->tabs->addTab(this->results_tab, tr("Results"));
}

/**
 * @brief      Build the drop-down menus
 */
void MainWindow::build_menu() {
    // build drop down menus
    QMenuBar *menuBar = new QMenuBar;
    QMenu *menuFile = menuBar->addMenu(tr("&File"));
    QMenu *menuHelp = menuBar->addMenu(tr("&Help"));

    // build menu bar items
    QAction *action_open = new QAction(menuFile);
    QAction *action_save = new QAction(menuFile);
    QAction *action_quit = new QAction(menuFile);
    QAction *action_about = new QAction(menuHelp);

    // define actions
    action_open->setText(tr("Open"));
    action_open->setShortcuts(QKeySequence::Open);

    action_save->setText(tr("Save"));
    action_save->setShortcuts(QKeySequence::Save);

    action_quit->setText(tr("Quit"));
    action_quit->setShortcuts(QKeySequence::Quit);

    action_about->setText(tr("About"));

    // add options to menu
    menuFile->addAction(action_open);
    menuFile->addAction(action_save);
    menuFile->addAction(action_quit);
    menuHelp->addAction(action_about);

    // connect actions
    connect(action_quit, &QAction::triggered, this, &MainWindow::exit);

    setMenuBar(menuBar);
}
