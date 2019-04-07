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

#ifndef _RENDER_ARA
#define _RENDER_ARA

#include <QWidget>
#include <QPainter>
#include <QPixmap>

#include "two_dim_rd.h"
#include "reaction_lotka_volterra.h"
#include "colorschemes/viridis.h"
#include "colorschemes/magma.h"

class RenderArea : public QWidget {
    Q_OBJECT

private:
    std::vector<QPixmap> graphs;
    unsigned int ctr;
    const std::vector<float>* color_scheme;

public:
    /**
     * @brief      Constructs the object.
     *
     * @param      parent  The parent
     */
    explicit RenderArea(QWidget *parent = 0);

    QSize minimumSizeHint() const override;

    QSize sizeHint() const override;

    void next_img();

    void prev_img();

    /**
     * @brief      Gets the counter.
     *
     * @return     The counter.
     */
    inline unsigned int get_ctr() const {
        return this->ctr;
    }

    /**
     * @brief      Sets the counter.
     *
     * @param[in]  _ctr  The counter
     */
    inline void set_ctr(unsigned int _ctr) {
        this->ctr = _ctr;
        this->update();
    }

    /**
     * @brief      Gets the number of graphs.
     *
     * @return     The number of graphs.
     */
    inline unsigned int get_num_graphs() const {
        return this->graphs.size();
    }

    /**
     * @brief      Adds a graph.
     *
     * @param[in]  X     Concentrations
     * @param[in]  mask  The mask
     */
    void add_graph(const MatrixXXd& X, const MatrixXXi& mask);

    /**
     * @brief      Sets the color scheme.
     *
     * @param[in]  _color_scheme  The color scheme
     */
    inline void set_color_scheme(const std::vector<float>* _color_scheme) {
        this->color_scheme = _color_scheme;
    }

    /**
     * @brief      Clear all results
     */
    void clear();

protected:
    /**
     * @brief      Perform paint event call
     *
     * @param      event  The paint event
     */
    void paintEvent(QPaintEvent *event) override;

private:
    /**
     * @brief      Convert raw concentration data to color graph
     *
     * @param[in]  data  The raw concentration data
     * @param[in]  mask  The mask
     *
     * @return     ByteArray with colors
     */
    std::vector<uint8_t> convert_data(const MatrixXXd& data, const MatrixXXi& mask) const;

    /**
     * @brief      Obtain color from data point using color scheme
     *
     * @param[in]  val     The value
     * @param[in]  minval  Minimum value
     * @param[in]  maxval  Maximum value
     *
     * @return     The color.
     */
    std::array<uint8_t, 3> get_color(double val, double minval, double maxval) const;

private slots:

};

#endif // _RENDER_ARA
