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

#include "renderarea.h"

/**
 * @brief Results tab constructor
 * @param parent widget
 */
RenderArea::RenderArea(QWidget *parent) : QWidget(parent) {
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);

    this->rd_system = std::make_shared<TwoDimRD>(2e-5, 1e-5, 256, 256, 0.005, 0.01, 100, 10);
    this->rd_system->set_reaction(dynamic_cast<ReactionSystem*>(new ReactionLotkaVolterra()));
    this->rd_system->set_pbc(true);
    this->rd_system->set_parameters("alpha=2.3333;beta=2.6666;gamma=1.0;delta=1.0");
    this->rd_system->time_integrate();

    // std::cout << "Constructing and saving pixmap" << std::endl;
    for(unsigned int i=0; i<this->rd_system->get_num_img(); i++) {
        QByteArray data = this->rd_system->get_qbyte_array(i);
        QImage img((const uchar*)(data.constData()), 256, 256, QImage::Format_RGB888);
        this->graphs.push_back(QPixmap::fromImage(img));
    }

    this->update();
}

QSize RenderArea::sizeHint() const {
    return QSize(512, 512);
}

QSize RenderArea::minimumSizeHint() const {
    return QSize(256, 256);
}

void RenderArea::paintEvent(QPaintEvent * /* event */) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(palette().dark().color());
    painter.setBrush(Qt::NoBrush);
    painter.drawPixmap(0, 0, width() - 1, height() - 1, this->graphs.back(), 0, 0, this->graphs.back().width(), this->graphs.back().height());
    painter.drawRect(QRect(0, 0, width() - 1, height() - 1));
}
