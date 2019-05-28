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

/**
 * @brief      Show next image
 */
void RenderArea::next_img() {
    this->ctr++;
    if(this->ctr >= this->graphs.size()) {
        this->ctr = 0;
    }
    this->update();
}

/**
 * @brief      Show previous image
 */
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
 * @param[in]  mask  The mask
 */
void RenderArea::add_graph(const MatrixXXd& data, const MatrixXXi& mask) {
    std::vector<uint8_t> graph_data;
    if(mask.cols() != data.cols() || mask.rows() != data.rows()) {
        graph_data = this->convert_data(data, MatrixXXi::Zero(data.rows(), data.cols()));
    } else {
        graph_data = this->convert_data(data, mask);
    }

    unsigned int width = data.cols() + (data.cols() * 3) % 4;
    unsigned int height = data.rows() + (data.rows() * 3) % 4;

    QImage img(&graph_data[0], width, height, QImage::Format_RGB888);
    this->graphs.push_back(QPixmap::fromImage(img));
}

/**
 * @brief      Saves an image from the graph
 *
 * @param[in]  graph_id  The graph identifier
 * @param[in]  filename  The filename
 */
void RenderArea::save_image(unsigned int graph_id, const QString& filename) {
    if(graph_id < this->graphs.size()) {
        QFile file(filename);
        file.open(QIODevice::WriteOnly);
        this->graphs[graph_id].save(&file, "PNG");
    } else {
        throw std::logic_error("Invalid graph save request; graph id exceeds vector size.");
    }
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
 * @brief      Clear all results
 */
void RenderArea::clear() {
    this->graphs.clear();
    this->ctr = 0;
}

/**
 * @brief      Gets the current image.
 *
 * @return     The current image.
 */
const QPixmap& RenderArea::get_current_image() const {
    if(this->ctr >= this->graphs.size()) {
        throw std::runtime_error("Cannot return current image");
    }
    return this->graphs[this->ctr];
}

/**
 * @brief      Convert raw concentration data to color graph
 *
 * @param[in]  data  The raw concentration data
 *
 * @return     ByteArray with colors
 */
std::vector<uint8_t> RenderArea::convert_data(const MatrixXXd& data, const MatrixXXi& mask) const {
    double minval = 0.0;
    double maxval = 0.0;

    if(flag_boundary_values) {
        minval = this->graphs_minval;
        maxval = this->graphs_maxval;
    } else {
        minval = data.minCoeff();
        maxval = data.maxCoeff();
    }

    std::vector<uint8_t> result;
    // Qt Images need to be 32-bits aligned
    unsigned int nrows = data.rows() + (data.rows() * 3) % 4;
    unsigned int ncols = data.cols() + (data.cols() * 3) % 4;

    for(unsigned int y=0; y<nrows; y++) {
        for(unsigned int x=0; x<ncols; x++) {
            if(y >= data.rows() || x >= data.cols()) {
                result.push_back(0);
                result.push_back(0);
                result.push_back(0);
                continue;
            }

            if(mask(data.rows() - y - 1, x) == 1) {
                result.push_back(0);
                result.push_back(0);
                result.push_back(0);
                continue;
            }

            auto cols = this->color_scheme->get_color(data(data.rows() - y - 1, x), minval, maxval);
            result.push_back(cols[0]);
            result.push_back(cols[1]);
            result.push_back(cols[2]);
        }
    }

    return result;
}
