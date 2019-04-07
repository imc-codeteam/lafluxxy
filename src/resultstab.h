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

#ifndef _RESULTSTAB_H
#define _RESULTSTAB_H

#include <QWidget>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QLabel>
#include <QToolButton>
#include <QStyle>
#include <QProgressBar>

#include "renderarea.h"

class ResultsTab : public QWidget {
    Q_OBJECT

private:
    RenderArea *renderarea_X;
    RenderArea *renderarea_Y;

    QToolButton *button_next;
    QToolButton *button_prev;
    QToolButton *button_first;
    QToolButton *button_last;

    QProgressBar *progress_bar;
    QLabel *label_frame;
    QToolButton *button_stop;
    QLabel *label_integration_time;

    TwoDimRD* reaction_system;

    std::vector<double> dts;

public:
    /**
     * @brief Input tab constructor
     * @param parent widget
     */
    explicit ResultsTab(QWidget *parent = 0);

    /**
     * @brief      Update progress bar
     *
     * @param[in]  i      Current integration frame number
     * @param[in]  total  Total number of frames
     */
    void update_progress(unsigned int i, unsigned int total);

    /**
     * @brief      Sets the reaction system.
     *
     * @param      _reaction_system  The reaction system
     */
    inline void set_reaction_system(TwoDimRD* _reaction_system) {
        this->reaction_system = _reaction_system;
    }

    /**
     * @brief      Gets the stop button.
     *
     * @return     The stop button.
     */
    inline QToolButton* get_stop_button() const {
        return this->button_stop;
    }

    /**
     * @brief      Adds a frame.
     *
     * @param[in]  i     Frame index
     * @param[in]  dt    Wall clock integration time
     */
    void add_frame(unsigned int i, double dt = 0.0);

    /**
     * @brief      Clear all previous results
     */
    void clear();

    /**
     * @brief      Get number of frames stored
     *
     * @return     The number frames.
     */
    inline unsigned int get_num_frames() const {
        return this->renderarea_X->get_num_graphs();
    }

private:
    void update_frame_label();

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
};

#endif // _RESULTSTAB_H
