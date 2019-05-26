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
#include <QFile>

#include "two_dim_rd.h"
#include "reaction_lotka_volterra.h"
#include "colorscheme.h"

class RenderArea : public QWidget {
    Q_OBJECT

private:
    std::vector<QPixmap> graphs;
    unsigned int ctr;
    std::unique_ptr<ColorScheme> color_scheme;

    bool flag_boundary_values = false;
    double graphs_minval = 0.0;
    double graphs_maxval = 0.0;

public:
    /**
     * @brief      Constructs the object.
     *
     * @param      parent  The parent
     */
    explicit RenderArea(QWidget *parent = 0);

    QSize minimumSizeHint() const override;

    QSize sizeHint() const override;

    /**
     * @brief      Show next image
     */
    void next_img();

    /**
     * @brief      Show previous image
     */
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
     * @param[in]  data  Raw concentration data
     * @param[in]  mask  The mask
     *
     * Note: Pass the data object NOT as a const reference, but as a copy!
     *       Due to the multi-threaded nature of the application, the vector
     *       containing the original data can be resized, while the data
     *       still needs to be parsed!
     */
    void add_graph(MatrixXXd data, const MatrixXXi& mask);

    /**
     * @brief      Saves an image from the graph
     *
     * @param[in]  graph_id  The graph identifier
     * @param[in]  filename  The filename
     */
    void save_image(unsigned int graph_id, const QString& filename);

    /**
     * @brief      Sets the color scheme.
     *
     * @param[in]  name  The name
     */
    inline void set_color_scheme(const std::string& name) {
        this->color_scheme = std::make_unique<ColorScheme>(name);
    }

    /**
     * @brief      Clear all results
     */
    void clear();

    /**
     * @brief      Gets the current image.
     *
     * @return     The current image.
     */
    const QPixmap& get_current_image() const;

    /**
     * @brief      Sets the minimum value for the coloring of the graphs
     *
     * @param[in]  _minval  The minval
     */
    inline void set_minval(double _minval) {
        this->graphs_minval = _minval;
    }

    /**
     * @brief      Sets the maximum value for the coloring of the graphs
     *
     * @param[in]  _maxval  The maxval
     */
    inline void set_maxval(double _maxval) {
        this->graphs_maxval = _maxval;
    }

    /**
     * @brief      Set flag whether to use use-supplied boundary values
     *
     * @param[in]  _boundary_values  Whether to use use-supplied boundary values
     */
    inline void use_boundary_values(bool _boundary_values) {
        this->flag_boundary_values = _boundary_values;
    }

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

private slots:

};

#endif // _RENDER_ARA
