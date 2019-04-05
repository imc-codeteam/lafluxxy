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

    QImage img((const uchar*)(graph_data.constData()), data.cols(), data.rows(), QImage::Format_RGB888);
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
    const double minval = data.minCoeff();
    const double maxval = data.maxCoeff();

    QByteArray result;
    for(unsigned int y=0; y<data.rows(); y++) {
        for(unsigned int x=0; x<data.cols(); x++) {
            auto cols = this->get_color(data(data.rows() - y - 1,x), minval, maxval);
            result.push_back(cols[0]);
            result.push_back(cols[1]);
            result.push_back(cols[2]);
        }
    }

    return result;
}

/**
 * @brief      Obtain color from data point using color scheme
 *
 * @param[in]  val     The value
 * @param[in]  minval  Minimum value
 * @param[in]  maxval  Maximum value
 *
 * @return     The color.
 */
std::array<uint8_t, 3> RenderArea::get_color(double val, double minval, double maxval) const {
    if(val <= minval) {
        return std::array<uint8_t, 3>{uint8_t(this->color_scheme->at(0) * 256.0f), uint8_t(this->color_scheme->at(1) * 256.0f), uint8_t(this->color_scheme->at(2) * 256.0f)};
    }

    if(val >= maxval) {
        const unsigned int sz = this->color_scheme->size();
        return std::array<uint8_t, 3>{uint8_t(this->color_scheme->at(sz-3) * 256.0f), uint8_t(this->color_scheme->at(sz-2) * 256.0f), uint8_t(this->color_scheme->at(sz-1) * 256.0f)};
    }

    unsigned int idx = (val - minval)/(maxval - minval) * (this->color_scheme->size() / 3);

    return std::array<uint8_t, 3>{uint8_t(this->color_scheme->at(idx*3) * 256.0f), uint8_t(this->color_scheme->at(idx*3+1) * 256.0f), uint8_t(this->color_scheme->at(idx*3+2) * 256.0f)};
}
