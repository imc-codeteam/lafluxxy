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
    this->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);

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
    scroll_area->setWidget(widget);

    // set concentrations layout
    QGridLayout *concentrations_layout = new QGridLayout;
    widget->setLayout(concentrations_layout);

    concentrations_layout->addWidget(new QLabel(tr("Concentration X")), 0, 0);
    this->renderarea_X = new RenderArea();
    this->renderarea_X->set_color_scheme("viridis");
    concentrations_layout->addWidget(this->renderarea_X, 1, 0);

    concentrations_layout->addWidget(new QLabel(tr("Concentration Y")), 0, 2);
    this->renderarea_Y = new RenderArea();
    this->renderarea_Y->set_color_scheme("piyg");
    concentrations_layout->addWidget(this->renderarea_Y, 1, 2);

    this->button_save_image_X = new QToolButton(this);
    this->button_save_image_X->setIcon(style()->standardIcon(QStyle::SP_DialogSaveButton));
    concentrations_layout->addWidget(this->button_save_image_X, 1, 1);
    connect(this->button_save_image_X, SIGNAL(clicked()), this, SLOT(save_concentration_X()));

    this->button_save_image_Y = new QToolButton(this);
    this->button_save_image_Y->setIcon(style()->standardIcon(QStyle::SP_DialogSaveButton));
    concentrations_layout->addWidget(this->button_save_image_Y, 1, 3);
    connect(this->button_save_image_Y, SIGNAL(clicked()), this, SLOT(save_concentration_Y()));

    concentrations_layout->addWidget(new QLabel(tr("Fourier Transformation X")), 2, 0);
    this->renderarea_ft_X = new RenderArea();
    this->renderarea_ft_X->set_color_scheme("spectral");
    concentrations_layout->addWidget(this->renderarea_ft_X, 3, 0);

    concentrations_layout->addWidget(new QLabel(tr("Fourier Transformation Y")), 2, 2);
    this->renderarea_ft_Y = new RenderArea();
    this->renderarea_ft_Y->set_color_scheme("spectral");
    concentrations_layout->addWidget(this->renderarea_ft_Y, 3, 2);

    this->button_save_ftimage_X = new QToolButton(this);
    this->button_save_ftimage_X->setIcon(style()->standardIcon(QStyle::SP_DialogSaveButton));
    concentrations_layout->addWidget(this->button_save_ftimage_X, 3, 1);
    connect(this->button_save_ftimage_X, SIGNAL(clicked()), this, SLOT(save_ft_X()));

    this->button_save_ftimage_Y = new QToolButton(this);
    this->button_save_ftimage_Y->setIcon(style()->standardIcon(QStyle::SP_DialogSaveButton));
    concentrations_layout->addWidget(this->button_save_ftimage_Y, 3, 3);
    connect(this->button_save_ftimage_Y, SIGNAL(clicked()), this, SLOT(save_ft_Y()));

    // set up frame interface
    this->slider_frame = new QSlider(Qt::Horizontal);
    main_layout->addWidget(this->slider_frame);
    this->slider_frame->setMinimum(0);
    this->slider_frame->setMaximum(0);
    this->slider_frame->setTickPosition(QSlider::TicksBelow);
    connect(this->slider_frame, SIGNAL(valueChanged(int)), this, SLOT(slider_moved(int)));

    QWidget *gridwidget = new QWidget;
    QGridLayout *gridlayout = new QGridLayout;
    gridwidget->setLayout(gridlayout);

    this->button_first = new QToolButton(this);
    this->button_first->setIcon(style()->standardIcon(QStyle::SP_MediaSkipBackward));
    gridlayout->addWidget(this->button_first, 0, 0);
    connect(this->button_first, SIGNAL(clicked()), this, SLOT(goto_first()));

    this->button_prev = new QToolButton(this);
    this->button_prev->setIcon(style()->standardIcon(QStyle::SP_MediaSeekBackward));
    gridlayout->addWidget(this->button_prev, 0, 1);
    connect(this->button_prev, SIGNAL(clicked()), this, SLOT(prev_img()));

    this->button_next = new QToolButton(this);
    this->button_next->setIcon(style()->standardIcon(QStyle::SP_MediaSeekForward));
    gridlayout->addWidget(this->button_next, 0, 2);
    connect(this->button_next, SIGNAL(clicked()), this, SLOT(next_img()));

    this->button_last = new QToolButton(this);
    this->button_last->setIcon(style()->standardIcon(QStyle::SP_MediaSkipForward));
    gridlayout->addWidget(this->button_last, 0, 3);
    connect(this->button_last, SIGNAL(clicked()), this, SLOT(goto_last()));

    this->label_frame = new QLabel(this);
    gridlayout->addWidget(this->label_frame, 0, 4);
    main_layout->addWidget(gridwidget);

    this->button_copy_to_movie = new QPushButton("Copy data to movie tab");
    main_layout->addWidget(this->button_copy_to_movie);
    this->button_copy_to_movie->setEnabled(false);

    // show integration time statistics
    QWidget *time_integration_widget = new QWidget();
    time_integration_widget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    this->layout_integration_times = new QGridLayout();
    this->layout_integration_times->setColumnStretch(1, 10);
    time_integration_widget->setLayout(this->layout_integration_times);
    main_layout->addWidget(time_integration_widget);
    this->layout_integration_times->addWidget(new QLabel("<html><b>Integration time</b></html>"), 0, 0);
    this->layout_integration_times->addWidget(new QLabel("Last frame:"), 1, 0);
    this->layout_integration_times->addWidget(new QLabel("Average:"), 2, 0);
    this->layout_integration_times->addWidget(new QLabel("Total time:"), 3, 0);
    this->layout_integration_times->addWidget(new QLabel("Estimated remaining:"), 4, 0);

    this->label_time_last_frame = new QLabel();
    this->label_time_average = new QLabel();
    this->label_time_remaining = new QLabel();
    this->label_time_total = new QLabel();
    this->layout_integration_times->addWidget(this->label_time_last_frame, 1, 1);
    this->layout_integration_times->addWidget(this->label_time_average, 2, 1);
    this->layout_integration_times->addWidget(this->label_time_total, 3, 1);
    this->layout_integration_times->addWidget(this->label_time_remaining, 4, 1);

    // set up progress bar
    main_layout->addWidget(new QLabel("<b>Simulation progress</b>"));
    QWidget *progress_widget = new QWidget;
    QGridLayout *progress_layout = new QGridLayout;
    progress_widget->setLayout(progress_layout);
    this->progress_bar = new QProgressBar;
    progress_layout->addWidget(this->progress_bar, 0, 0);
    this->button_stop = new QToolButton(this);
    this->button_stop->setIcon(style()->standardIcon(QStyle::SP_DialogCancelButton));
    this->button_stop->setWhatsThis(tr("Cancel simulation"));
    this->button_stop->setEnabled(false);
    progress_layout->addWidget(this->button_stop, 0, 1);
    main_layout->addWidget(progress_widget);
}

/**
 * @brief      Update progress bar
 *
 * @param[in]  i      Current integration frame number
 * @param[in]  total  Total number of frames
 */
void ResultsTab::update_progress(unsigned int i, unsigned int total) {
    this->progress_bar->setRange(0, total);
    this->progress_bar->setValue(i);
}

/**
 * @brief      Adds a frame.
 *
 * @param[in]  i     Frame index
 * @param[in]  dt    Wall clock integration time
 */
void ResultsTab::add_frame(unsigned int i, double dt) {
    auto data_X = this->reaction_system->get_concentration_matrix(i, true);
    auto data_Y = this->reaction_system->get_concentration_matrix(i, false);

    this->renderarea_X->add_graph(data_X, this->reaction_system->get_mask());
    this->renderarea_Y->add_graph(data_Y, this->reaction_system->get_mask());

    // create Fourier transforms
    this->construct_ft(data_X, this->renderarea_ft_X);
    this->construct_ft(data_Y, this->renderarea_ft_Y);

    // update render area
    if(i == 0) {
        this->renderarea_X->update();
        this->renderarea_Y->update();

        this->renderarea_ft_X->update();
        this->renderarea_ft_Y->update();
    }

    // update running time
    if(i != 0) {
        this->dts.push_back(dt);
        this->total_t += dt;
        double avg = std::accumulate(this->dts.begin(), this->dts.end(), 0.0) / (double)dts.size();
        double remaining = avg * (this->progress_bar->maximum() - this->progress_bar->value());
        this->label_time_last_frame->setText(QString::number(this->dts.back()) + tr(" sec"));
        this->label_time_average->setText(QString::number(avg) + tr(" sec"));
        this->label_time_remaining->setText(QString::number(remaining) + tr(" sec"));
        this->label_time_total->setText(QString::number(this->total_t) + tr(" sec"));
    }

    // update slider
    this->update_slider_frame();

    // update frame label
    this->update_frame_label();
}

/**
 * @brief      Clear all previous results
 */
void ResultsTab::clear() {
    this->dts.clear();
    this->total_t = 0.0;
    this->renderarea_X->clear();
    this->renderarea_Y->clear();
    this->label_time_last_frame->setText("");
    this->label_time_average->setText("");
    this->label_time_remaining->setText("");
    this->label_time_total->setText("");
    this->button_copy_to_movie->setEnabled(false);
}

/**
 * @brief      Update frame label
 */
void ResultsTab::update_frame_label() {
    if(this->renderarea_X->get_num_graphs() > 0) {
        this->button_copy_to_movie->setEnabled(true);
    }
    this->label_frame->setText(tr("Frame: ") + QString::number(this->renderarea_X->get_ctr()+1) + "/" + QString::number(this->renderarea_X->get_num_graphs()));
}

/**
 * @brief      Update the slider
 */
void ResultsTab::update_slider_frame() {
    const unsigned int num_graphs = this->renderarea_X->get_num_graphs();
    this->slider_frame->setMinimum(1);
    this->slider_frame->setMaximum(num_graphs);
    this->slider_frame->setValue(this->renderarea_X->get_ctr()+1);
    if(num_graphs < 20) {
        this->slider_frame->setTickInterval(1);
    } else if(num_graphs < 50) {
        this->slider_frame->setTickInterval(2);
    }  else if(num_graphs < 100) {
        this->slider_frame->setTickInterval(5);
    } else {
        this->slider_frame->setTickInterval(num_graphs / 20);
    }
}

/**
 * @brief      Saves an image.
 *
 * @param[in]  img   The image
 */
void ResultsTab::save_image(const QPixmap& img) {
    QString filename = QFileDialog::getSaveFileName(this, tr("Save File"), "", tr("Images (*.png)"));
    img.save(filename, "PNG");
}

/**
 * @brief      Show next time frame
 */
void ResultsTab::next_img() {
    this->renderarea_X->next_img();
    this->renderarea_Y->next_img();
    this->update_frame_label();
    this->update_slider_frame();
}

/**
 * @brief      Show previous time frame
 */
void ResultsTab::prev_img() {
    this->renderarea_X->prev_img();
    this->renderarea_Y->prev_img();
    this->renderarea_ft_X->prev_img();
    this->renderarea_ft_Y->prev_img();
    this->update_frame_label();
    this->update_slider_frame();
}

/**
 * @brief      Show first frame
 */
void ResultsTab::goto_first() {
    this->renderarea_X->set_ctr(0);
    this->renderarea_Y->set_ctr(0);
    this->renderarea_ft_X->set_ctr(0);
    this->renderarea_ft_Y->set_ctr(0);
    this->update_frame_label();
    this->update_slider_frame();
}

/**
 * @brief      Show last frame
 */
void ResultsTab::goto_last() {
    this->renderarea_X->set_ctr(this->renderarea_X->get_num_graphs()-1);
    this->renderarea_Y->set_ctr(this->renderarea_Y->get_num_graphs()-1);
    this->renderarea_ft_X->set_ctr(this->renderarea_ft_X->get_num_graphs()-1);
    this->renderarea_ft_Y->set_ctr(this->renderarea_ft_Y->get_num_graphs()-1);
    this->update_frame_label();
    this->update_slider_frame();
}

/**
 * @brief      Execute when slider is moved
 */
void ResultsTab::slider_moved(int value) {
    this->renderarea_X->set_ctr(value - 1);
    this->renderarea_Y->set_ctr(value - 1);
    this->renderarea_ft_X->set_ctr(value - 1);
    this->renderarea_ft_Y->set_ctr(value - 1);
    this->update_frame_label();
}

/**
 * @brief      Save the concentration profile of X to a file
 */
void ResultsTab::save_concentration_X() {
    try {
        this->save_image(this->renderarea_X->get_current_image());
    } catch(const std::exception& e) {
        return;
    }
}

/**
 * @brief      Save the concentration profile of X to a file
 */
void ResultsTab::save_concentration_Y() {
    try {
        this->save_image(this->renderarea_Y->get_current_image());
    } catch(const std::exception& e) {
        return;
    }
}

/**
 * @brief      Save the ft profile of X to a file
 */
void ResultsTab::save_ft_X() {
    try {
        this->save_image(this->renderarea_ft_X->get_current_image());
    } catch(const std::exception& e) {
        return;
    }
}

/**
 * @brief      Save the ft profile of X to a file
 */
void ResultsTab::save_ft_Y() {
    try {
        this->save_image(this->renderarea_ft_Y->get_current_image());
    } catch(const std::exception& e) {
        return;
    }
}

/**
 * @brief      Construct Fourier Transform
 *
 * @param[in]  data         The data
 * @param      destination  The destination
 */
void ResultsTab::construct_ft(MatrixXXd data, RenderArea* destination) {
    unsigned int rowsize = data.rows();
    unsigned int colsize = data.cols();
    unsigned int halfrowsize = rowsize / 2;
    unsigned int halfcolsize = colsize / 2;
    unsigned int ftcolsize = colsize / 2 + 1;

    fftw_complex *ft_data = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * data.rows() * data.cols());
    fftw_plan plan = fftw_plan_dft_r2c_2d(data.cols(), data.rows(), data.data(), ft_data, FFTW_ESTIMATE);
    fftw_execute(plan);

    MatrixXXd ft_data_mat(data.cols(), data.rows());

    // copy absolute values of FT to matrix for concentration X and transform the data
    for(unsigned int i=0; i < rowsize; i++) {
        for(unsigned int j=0; j < halfcolsize; j++) {
            double xreal = ft_data[i * ftcolsize + j][0];
            double xcomplex = ft_data[i * ftcolsize + j][1];
            ft_data_mat(std::fmod(i + halfrowsize, rowsize), halfcolsize - j - 1) = xreal * xreal + xcomplex * xcomplex;
            ft_data_mat(std::fmod(i + halfrowsize, rowsize), j + halfcolsize) = xreal * xreal + xcomplex * xcomplex;
        }
    }

    destination->set_minval(0.0);
    destination->set_maxval((double)(data.cols() * data.rows()));
    destination->use_boundary_values(true);
    destination->add_graph(ft_data_mat, MatrixXXi::Zero(data.cols(), data.rows()));

    fftw_destroy_plan(plan);
    fftw_free(ft_data);
}
