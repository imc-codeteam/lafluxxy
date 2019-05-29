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

#include "movietab.h"

MovieTab::MovieTab(QWidget* parent) : QWidget(parent) {
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

    // left hand side

    concentrations_layout->addWidget(new QLabel(tr("Concentration X")), 0, 0);
    this->renderarea_X = new RenderArea();
    this->renderarea_X->set_color_scheme("viridis");
    concentrations_layout->addWidget(this->renderarea_X, 1, 0);

    concentrations_layout->addWidget(new QLabel("Minimum value for X"), 2, 0);
    this->value_min_x = new QDoubleSpinBox();
    this->value_min_x->setDecimals(3);
    concentrations_layout->addWidget(this->value_min_x, 3, 0);
    concentrations_layout->addWidget(new QLabel("Maximum value for X"), 4, 0);
    this->value_max_x = new QDoubleSpinBox();
    this->value_max_x->setDecimals(3);
    concentrations_layout->addWidget(this->value_max_x, 5, 0);
    this->color_scheme_x = this->build_color_scheme_selector();
    concentrations_layout->addWidget(this->color_scheme_x, 6, 0);

    // right hand side

    concentrations_layout->addWidget(new QLabel(tr("Concentration Y")), 0, 1);
    this->renderarea_Y = new RenderArea();
    this->renderarea_Y->set_color_scheme("piyg");
    concentrations_layout->addWidget(this->renderarea_Y, 1, 1);

    concentrations_layout->addWidget(new QLabel("Minimum value for Y"), 2, 1);
    this->value_min_y = new QDoubleSpinBox();
    this->value_min_y->setDecimals(3);
    concentrations_layout->addWidget(this->value_min_y, 3, 1);
    concentrations_layout->addWidget(new QLabel("Maximum value for Y"), 4, 1);
    this->value_max_y = new QDoubleSpinBox();
    this->value_max_y->setDecimals(3);
    concentrations_layout->addWidget(this->value_max_y, 5, 1);
    this->color_scheme_y = this->build_color_scheme_selector();
    concentrations_layout->addWidget(this->color_scheme_y, 6, 1);
    this->color_scheme_y->setCurrentText("PiYG");

    // set up frame interface
    this->slider_frame = new QSlider(Qt::Horizontal);
    main_layout->addWidget(this->slider_frame);
    this->slider_frame->setMinimum(0);
    this->slider_frame->setMaximum(0);
    this->slider_frame->setTickPosition(QSlider::TicksBelow);
    connect(this->slider_frame, SIGNAL(valueChanged(int)), this, SLOT(slider_moved(int)));

    // add button container
    QWidget* button_widget = new QWidget();
    main_layout->addWidget(button_widget);
    QHBoxLayout* button_layout = new QHBoxLayout();
    button_widget->setLayout(button_layout);

    // add button to rebuild all graphs
    this->button_rebuild_graphs = new QPushButton(" Rebuild graphs");
    QIcon icon_button_rebuild_graphs = style()->standardIcon(QStyle::SP_BrowserReload);
    this->button_rebuild_graphs->setIcon(icon_button_rebuild_graphs);
    this->button_rebuild_graphs->setToolTip("Rebuild all graphs using custom color schemes.");
    button_layout->addWidget(this->button_rebuild_graphs);
    connect(this->button_rebuild_graphs, SIGNAL(released()), this, SLOT(rebuild_graphs()));

    // add button to save all movie files
    this->button_save_image_files = new QPushButton(" Save images");
    QIcon icon_button_save_image_files = style()->standardIcon(QStyle::SP_DialogSaveButton);
    this->button_save_image_files->setIcon(icon_button_save_image_files);
    this->button_save_image_files->setToolTip("Save all image files to custom folder.");
    button_layout->addWidget(this->button_save_image_files);
    connect(this->button_save_image_files, SIGNAL(released()), this, SLOT(save_images()));

    // add button to save raw data
    this->button_save_raw_data = new QPushButton(" Save raw data");
    QIcon icon_button_save_raw_data = style()->standardIcon(QStyle::SP_FileIcon);
    this->button_save_raw_data->setIcon(icon_button_save_raw_data);
    this->button_save_raw_data->setToolTip("Save raw data file (compressed file format).");
    button_layout->addWidget(this->button_save_raw_data);
    connect(this->button_save_raw_data, SIGNAL(released()), this, SLOT(save_raw_data()));

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
}

void MovieTab::set_concentrations(const std::vector<MatrixXXd>& _conc_X, const std::vector<MatrixXXd>& _conc_Y, const MatrixXXi& _mask) {
    this->clear();
    this->concentrations_X = _conc_X;
    this->concentrations_Y = _conc_Y;
    this->mask = _mask;

    double minval_x =_conc_X.back().minCoeff();
    double maxval_x =_conc_X.back().maxCoeff();

    this->renderarea_X->set_minval(minval_x);
    this->renderarea_X->set_maxval(maxval_x);
    this->renderarea_X->use_boundary_values(true);
    this->value_min_x->setValue(minval_x);
    this->value_max_x->setValue(maxval_x);

    double minval_y =_conc_Y.back().minCoeff();
    double maxval_y =_conc_Y.back().maxCoeff();

    this->renderarea_Y->set_minval(minval_y);
    this->renderarea_Y->set_maxval(maxval_y);
    this->renderarea_Y->use_boundary_values(true);
    this->value_min_y->setValue(minval_y);
    this->value_max_y->setValue(maxval_y);

    for(unsigned int i=0; i<this->concentrations_X.size(); i++) {
        this->renderarea_X->add_graph(this->concentrations_X[i], this->mask);
        this->renderarea_Y->add_graph(this->concentrations_Y[i], this->mask);
    }

    this->update_frame_label();
    this->update_slider_frame();
}

/**
 * @brief      Update frame label
 */
void MovieTab::update_frame_label() {
    this->label_frame->setText(tr("Frame: ") + QString::number(this->renderarea_X->get_ctr()+1) + "/" + QString::number(this->renderarea_X->get_num_graphs()));
}

/**
 * @brief      Update the slider
 */
void MovieTab::update_slider_frame() {
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
 * @brief      Builds a color scheme selector.
 *
 * @return     The color scheme selector.
 */
QComboBox* MovieTab::build_color_scheme_selector() {
    QComboBox* selector = new QComboBox();
    selector->addItem("Viridis");
    selector->addItem("Magma");
    selector->addItem("Plasma");
    selector->addItem("Inferno");
    selector->addItem("PiYG");
    selector->addItem("Spectral");

    return selector;
}

/**
 * @brief      Show next time frame
 */
void MovieTab::next_img() {
    this->renderarea_X->next_img();
    this->renderarea_Y->next_img();
    this->update_frame_label();
    this->update_slider_frame();
}

/**
 * @brief      Show previous time frame
 */
void MovieTab::prev_img() {
    this->renderarea_X->prev_img();
    this->renderarea_Y->prev_img();
    this->update_frame_label();
    this->update_slider_frame();
}

/**
 * @brief      Show first frame
 */
void MovieTab::goto_first() {
    this->renderarea_X->set_ctr(0);
    this->renderarea_Y->set_ctr(0);
    this->update_frame_label();
    this->update_slider_frame();
}

/**
 * @brief      Show last frame
 */
void MovieTab::goto_last() {
    this->renderarea_X->set_ctr(this->renderarea_X->get_num_graphs()-1);
    this->renderarea_Y->set_ctr(this->renderarea_Y->get_num_graphs()-1);
    this->update_frame_label();
    this->update_slider_frame();
}

/**
 * @brief      Execute when slider is moved
 */
void MovieTab::slider_moved(int value) {
    this->renderarea_X->set_ctr(value - 1);
    this->renderarea_Y->set_ctr(value - 1);
    this->update_frame_label();
}

/**
 * @brief      Clear all previous results
 */
void MovieTab::clear() {
    this->renderarea_X->clear();
    this->renderarea_Y->clear();
}

/**
 * @brief      Change boundary values
 */
void MovieTab::rebuild_graphs() {
    this->clear();

    this->renderarea_X->set_color_scheme(this->color_scheme_x->currentText().toLower().toStdString());
    this->renderarea_Y->set_color_scheme(this->color_scheme_y->currentText().toLower().toStdString());

    this->renderarea_X->set_minval(this->value_min_x->value());
    this->renderarea_X->set_maxval(this->value_max_x->value());
    this->renderarea_X->use_boundary_values(true);
    this->renderarea_Y->set_minval(this->value_min_y->value());
    this->renderarea_Y->set_maxval(this->value_max_y->value());
    this->renderarea_Y->use_boundary_values(true);

    for(unsigned int i=0; i<this->concentrations_X.size(); i++) {
        this->renderarea_X->add_graph(this->concentrations_X[i], this->mask);
        this->renderarea_Y->add_graph(this->concentrations_Y[i], this->mask);
    }

    this->goto_first();
    this->update_frame_label();
}

/**
 * @brief      Saves images.
 */
void MovieTab::save_images() {
    QFileDialog dialog_store_folder;
    dialog_store_folder.setFileMode(QFileDialog::Directory);
    dialog_store_folder.exec();
    QStringList selected_directory = dialog_store_folder.selectedFiles();

    if(selected_directory.isEmpty()) {
        return;
    }

    QDir output_folder(selected_directory[0]);

    for(unsigned int i=0; i<this->renderarea_X->get_num_graphs(); i++) {
        QString filename_A = output_folder.filePath((boost::format("A%03i.png") % i).str().c_str());
        this->renderarea_X->save_image(i, filename_A);
    }

    for(unsigned int i=0; i<this->renderarea_Y->get_num_graphs(); i++) {
        QString filename_B = output_folder.filePath((boost::format("B%03i.png") % i).str().c_str());
        this->renderarea_Y->save_image(i, filename_B);
    }
}

/**
 * @brief      Saves raw concentration data
 */
void MovieTab::save_raw_data() {
    if(this->concentrations_X.size() == 0 || this->concentrations_X.size() != this->concentrations_Y.size()) {
        return;
    }

    // QString filename = QFileDialog::getSaveFileName(this, tr("Save File"), "", tr("LaFluxxy Datapack (*.lfd)"));
    QFileDialog dialog_save_file;
    dialog_save_file.setDefaultSuffix(".lfd");
    dialog_save_file.setFileMode(QFileDialog::AnyFile);
    dialog_save_file.setAcceptMode(QFileDialog::AcceptSave);
    dialog_save_file.setLabelText(QFileDialog::LookIn ,tr("Select file to save simulation results to"));
    dialog_save_file.exec();
    QStringList selected_files = dialog_save_file.selectedFiles();

    if(selected_files.isEmpty()) {
        return;
    }

    boost::filesystem::path p(selected_files[0].toStdString());
    if(p.extension() == "") {
        p += ".lfd";
    }

    std::ofstream out(p.string());

    unsigned int rows = this->concentrations_X[0].rows();
    unsigned int cols = this->concentrations_X[0].cols();

    out << "# Datapack created in " << PROGRAM_NAME << " " << PROGRAM_VERSION << std::endl;
    out << "nframes = " << this->concentrations_X.size() << std::endl;
    out << "rows = " << rows << std::endl;
    out << "columns = " << cols << std::endl;
    out << "floatsize = " << (sizeof(MatrixXXd::Scalar) * 8) << std::endl;
    out << "vmin1 = " << this->value_min_x->value() << std::endl;
    out << "vmax1 = " << this->value_max_x->value() << std::endl;
    out << "vmin2 = " << this->value_min_y->value() << std::endl;
    out << "vmax2 = " << this->value_max_x->value() << std::endl;
    out << "end_header" << std::endl;

    for(unsigned int i=0; i<this->concentrations_X.size(); i++) {
        out.write((char*)this->concentrations_X[i].data(), rows * cols * sizeof(typename MatrixXXd::Scalar));
        out.write((char*)this->concentrations_Y[i].data(), rows * cols * sizeof(typename MatrixXXd::Scalar));
    }

    out.close();
}
