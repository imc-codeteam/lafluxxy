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

#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QLabel>
#include <QToolButton>
#include <QStyle>
#include <QProgressBar>
#include <QSlider>
#include <QFileDialog>
#include <QDoubleSpinBox>
#include <QPushButton>

#include "two_dim_rd.h"
#include "renderarea.h"

class MovieTab : public QWidget {
    Q_OBJECT

private:
    RenderArea *renderarea_X;
    RenderArea *renderarea_Y;

    QToolButton *button_next;
    QToolButton *button_prev;
    QToolButton *button_first;
    QToolButton *button_last;

    QSlider *slider_frame;
    QLabel *label_frame;

    QDoubleSpinBox *value_min_x;
    QDoubleSpinBox *value_max_x;
    QDoubleSpinBox *value_min_y;
    QDoubleSpinBox *value_max_y;

    QPushButton *button_rebuild_graphs;

    std::vector<MatrixXXd> concentrations_X;
    std::vector<MatrixXXd> concentrations_Y;
    MatrixXXi mask;

public:
    explicit MovieTab(QWidget *parent = 0);

    void set_concentrations(const std::vector<MatrixXXd>& _conc_X, const std::vector<MatrixXXd>& _conc_Y, const MatrixXXi& _mask);

private:
    /**
     * @brief      Update the label for the frame position
     */
    void update_frame_label();

    /**
     * @brief      Update the slider
     */
    void update_slider_frame();

private slots:
    /**
     * @brief      Show next time frame
     */
    void next_img();

    /**
     * @brief      Show previous time frame
     */
    void prev_img();

    /**
     * @brief      Show first frame
     */
    void goto_first();

    /**
     * @brief      Show last frame
     */
    void goto_last();

    /**
     * @brief      Execute when slider is moved
     */
    void slider_moved(int value);

    /**
     * @brief      Clear all previous results
     */
    void clear();

    /**
     * @brief      Rebuild all graphs
     */
    void rebuild_graphs();
};
