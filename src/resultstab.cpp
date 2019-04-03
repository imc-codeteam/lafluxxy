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

    QVBoxLayout *layout = new QVBoxLayout;
    widget->setLayout(layout);

    layout->addWidget(new QLabel(tr("Domain")));
    this->renderarea = new RenderArea();
    layout->addWidget(this->renderarea);

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

    layout->addWidget(gridwidget);

    this->progress_bar = new QProgressBar;
    layout->addWidget(this->progress_bar);
}

void ResultsTab::update_progress(unsigned int i, unsigned int total) {
    this->progress_bar->setRange(0, total);
    this->progress_bar->setValue(i);
}

void ResultsTab::next_img() {
    this->renderarea->next_img();
    this->frame_label->setText(QString::number(this->renderarea->get_ctr()+1) + "/" + QString::number(this->renderarea->get_num_graphs()));
}

void ResultsTab::prev_img() {
    this->renderarea->prev_img();
    this->frame_label->setText(QString::number(this->renderarea->get_ctr()+1) + "/" + QString::number(this->renderarea->get_num_graphs()));
}

void ResultsTab::add_frame(unsigned int i) {
    QByteArray data = this->reaction_system->get_qbyte_array(i);
    QImage img((const uchar*)(data.constData()), 256, 256, QImage::Format_RGB888);
    this->renderarea->add_graph(QPixmap::fromImage(img));

    // update render area
    if(i == 0) {
        this->renderarea->update();
    }
}
