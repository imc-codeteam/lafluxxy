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
#include <QSlider>
#include <QFileDialog>
#include <QPushButton>

#include <fftw3.h>

#include "renderarea.h"

class ResultsTab : public QWidget {
    Q_OBJECT

private:
    RenderArea *renderarea_X;
    RenderArea *renderarea_Y;

    RenderArea *renderarea_ft_X;
    RenderArea *renderarea_ft_Y;

    QToolButton *button_save_image_X;
    QToolButton *button_save_image_Y;

    QToolButton *button_next;
    QToolButton *button_prev;
    QToolButton *button_first;
    QToolButton *button_last;

    QToolButton *button_increase_size;
    QToolButton *button_decrease_size;

    QSlider *slider_frame;

    QProgressBar *progress_bar;
    QLabel *label_frame;
    QPushButton *button_copy_to_movie;

    QToolButton *button_stop;

    // integration time statistics
    QGridLayout *layout_integration_times;
    QLabel *label_time_last_frame;
    QLabel *label_time_average;
    QLabel *label_time_remaining;
    QLabel *label_time_total;

    TwoDimRD* reaction_system;

    std::vector<double> dts;
    double total_t = 0.0;

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
     * @brief      Gets the button copy to movie.
     *
     * @return     The button copy to movie.
     */
    inline QPushButton* get_button_copy_to_movie() const {
        return this->button_copy_to_movie;
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
    /**
     * @brief      Update the label for the frame position
     */
    void update_frame_label();

    /**
     * @brief      Update the slider
     */
    void update_slider_frame();

    /**
     * @brief      Saves an image.
     *
     * @param[in]  img   The image
     */
    void save_image(const QPixmap& img);

    /**
     * @brief      Construct Fourier Transform
     */
    void construct_ft(MatrixXXd data, RenderArea* destination);

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
     * @brief      Save the concentration profile of X to a file
     */
    void save_concentration_X();

    /**
     * @brief      Save the concentration profile of X to a file
     */
    void save_concentration_Y();
};

#endif // _RESULTSTAB_H
