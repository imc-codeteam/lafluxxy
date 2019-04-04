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

#include "resultstab.h"

/**
 * @brief Results tab constructor
 * @param parent widget
 */
ResultsTab::ResultsTab(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *main_layout = new QVBoxLayout;
    this->setLayout(main_layout);
    this->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

    // add a ScrollArea widget and define properties
    QScrollArea *scroll_area = new QScrollArea(this);     //Create scroll area Widget
    scroll_area->setContentsMargins(0,0,0,0);
    scroll_area->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    scroll_area->setWidgetResizable(true);

    // add ScrollArea to QWidget
    main_layout->addWidget(scroll_area);

    // create new Widget for in the QScrollArea and set properties
    QWidget* widget = new QWidget();
    widget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    // add Widget to ScrollArea
    scroll_area->setWidget(widget);

    // set concentrations layout
    QGridLayout *concentrations_layout = new QGridLayout;
    widget->setLayout(concentrations_layout);

    concentrations_layout->addWidget(new QLabel(tr("Concentration X")), 0, 0);
    this->renderarea_X = new RenderArea();
    concentrations_layout->addWidget(this->renderarea_X, 1, 0);

    concentrations_layout->addWidget(new QLabel(tr("Concentration Y")), 0, 1);
    this->renderarea_Y = new RenderArea();
    concentrations_layout->addWidget(this->renderarea_Y, 1, 1);

    // set up frame interface
    QWidget *gridwidget = new QWidget;
    QGridLayout *gridlayout = new QGridLayout;
    gridwidget->setLayout(gridlayout);

    this->button_prev = new QToolButton(this);
    this->button_prev->setIcon(style()->standardIcon(QStyle::SP_ArrowBack));
    gridlayout->addWidget(this->button_prev, 0, 0);
    connect(this->button_prev, SIGNAL(clicked()), this, SLOT(prev_img()));

    this->button_next = new QToolButton(this);
    this->button_next->setIcon(style()->standardIcon(QStyle::SP_ArrowForward));
    gridlayout->addWidget(this->button_next, 0, 1);
    connect(this->button_next, SIGNAL(clicked()), this, SLOT(next_img()));

    this->frame_label = new QLabel(this);
    gridlayout->addWidget(this->frame_label, 0, 2);

    main_layout->addWidget(gridwidget);

    // set up progress bar
    QWidget *progress_widget = new QWidget;
    QGridLayout *progress_layout = new QGridLayout;
    progress_widget->setLayout(progress_layout);
    this->progress_bar = new QProgressBar;
    progress_layout->addWidget(this->progress_bar, 0, 0);
    this->button_stop = new QToolButton(this);
    this->button_stop->setIcon(style()->standardIcon(QStyle::SP_DialogCancelButton));
    this->button_stop->setWhatsThis(tr("Cancel simulation"));
    this->button_stop->setEnabled(false);
    progress_layout->addWidget(this->button_stop, 0, 1);

    main_layout->addWidget(progress_widget);
}

void ResultsTab::update_progress(unsigned int i, unsigned int total) {
    this->progress_bar->setRange(0, total);
    this->progress_bar->setValue(i);
}

void ResultsTab::next_img() {
    this->renderarea_X->next_img();
    this->renderarea_Y->next_img();
    this->frame_label->setText(tr("Frame: ") + QString::number(this->renderarea_X->get_ctr()+1) + "/" + QString::number(this->renderarea_X->get_num_graphs()));
}

void ResultsTab::prev_img() {
    this->renderarea_X->prev_img();
    this->renderarea_Y->prev_img();
    this->frame_label->setText(tr("Frame: ") + QString::number(this->renderarea_X->get_ctr()+1) + "/" + QString::number(this->renderarea_X->get_num_graphs()));
}

void ResultsTab::add_frame(unsigned int i) {
    this->renderarea_X->add_graph(this->reaction_system->get_concentration_matrix(i, true));
    this->renderarea_Y->add_graph(this->reaction_system->get_concentration_matrix(i, false));

    // update render area
    if(i == 0) {
        this->renderarea_X->update();
        this->renderarea_Y->update();
    }
}
