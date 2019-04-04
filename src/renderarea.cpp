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
 * @brief      Constructs the object.
 *
 * @param      parent  The parent
 */
RenderArea::RenderArea(QWidget *parent) : QWidget(parent) {
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);

    this->ctr = 0;
    this->update();
}

QSize RenderArea::sizeHint() const {
    return QSize(512, 512);
}

QSize RenderArea::minimumSizeHint() const {
    return QSize(256, 256);
}

void RenderArea::next_img() {
    this->ctr++;
    if(this->ctr >= this->graphs.size()) {
        this->ctr = 0;
    }
    this->update();
}

void RenderArea::prev_img() {
    if(this->ctr == 0) {
        this->ctr = this->graphs.size() - 1;
    } else {
        this->ctr--;
    }
    this->update();
}

/**
 * @brief      Adds a graph.
 *
 * @param[in]  data  Raw concentration data
 */
void RenderArea::add_graph(const MatrixXXd& data) {
    QByteArray graph_data = this->convert_data(data);

    QImage img((const uchar*)(graph_data.constData()), 256, 256, QImage::Format_RGB888);
    this->graphs.push_back(QPixmap::fromImage(img));
}

/**
 * @brief      Perform paint event call
 *
 * @param      event  The paint event
 */
void RenderArea::paintEvent(QPaintEvent * /* event */) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(palette().dark().color());
    painter.setBrush(Qt::NoBrush);
    if(this->graphs.size() > 0) {
        painter.drawPixmap(0, 0, width() - 1, height() - 1, this->graphs[ctr], 0, 0, this->graphs[ctr].width(), this->graphs[ctr].height());
    }
    painter.drawRect(QRect(0, 0, width() - 1, height() - 1));
}

/**
 * @brief      Convert raw concentration data to color graph
 *
 * @param[in]  data  The raw concentration data
 *
 * @return     ByteArray with colors
 */
QByteArray RenderArea::convert_data(const MatrixXXd& data) const {
    QByteArray result;
    for(unsigned int y=0; y<data.rows(); y++) {
        for(unsigned int x=0; x<data.cols(); x++) {
            uchar val = (uint8_t)(data(data.rows() - y - 1,x) * 32.0);
            result.push_back(val);
            result.push_back(val);
            result.push_back(val);
        }
    }

    return result;
}
