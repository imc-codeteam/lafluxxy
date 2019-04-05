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

#include "inputtab.h"

/**
 * @brief Input tab constructor
 * @param parent widget
 */
InputTab::InputTab(QWidget *parent) : QWidget(parent), reaction_settings(nullptr) {
    QVBoxLayout *mainLayout = new QVBoxLayout;
    this->setLayout(mainLayout);
    this->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

    // add a ScrollArea widget and define properties
    QScrollArea *scrollArea = new QScrollArea(this);     //Create scroll area Widget
    scrollArea->setContentsMargins(0,0,0,0);
    scrollArea->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    scrollArea->setWidgetResizable(true);

    // add ScrollArea to QWidget
    mainLayout->addWidget(scrollArea);

    // create new Widget for in the QScrollArea and set properties
    QWidget* widget = new QWidget();
    widget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    // add Widget to ScrollArea
    scrollArea->setWidget(widget);

    QVBoxLayout *layout = new QVBoxLayout;
    widget->setLayout(layout);

    // add reaction settings
    QWidget* gridwidget_reaction = new QWidget();
    layout->addWidget(gridwidget_reaction);
    gridwidget_reaction->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    this->gridlayout_reaction = new QGridLayout();
    gridwidget_reaction->setLayout(this->gridlayout_reaction);
    this->gridlayout_reaction->setColumnStretch(1, 1);
    this->build_reaction_settings(this->gridlayout_reaction);

    // build general parameters
    QWidget* gridwidget = new QWidget();
    layout->addWidget(gridwidget);
    gridwidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    QGridLayout *gridlayout = new QGridLayout();
    gridwidget->setLayout(gridlayout);
    gridlayout->setColumnStretch(1, 1);
    this->build_general_parameters(gridlayout);

    // set launch button
    this->button_submit = new QPushButton("Launch calculation");
    layout->addWidget(this->button_submit);
    this->button_submit->setEnabled(false);

    // connect signals
    connect(this->reaction_selector, SIGNAL(currentIndexChanged(int)), SLOT(set_reaction_input(int)));
}

TwoDimRD* InputTab::build_reaction_system() {
    TwoDimRD* reaction_system = new TwoDimRD(this->input_diffusion_X->value(),
                                             this->input_diffusion_Y->value(),
                                             this->input_width->value(),
                                             this->input_height->value(),
                                             this->input_dx->value(),
                                             this->input_dt->value(),
                                             this->input_steps->value(),
                                             this->input_tsteps->value());
    reaction_system->set_cores(this->input_ncores->value());

    switch(this->reaction_selector->currentIndex()) {
        case LOTKA_VOLTERRA:
            reaction_system->set_reaction(dynamic_cast<ReactionSystem*>(new ReactionLotkaVolterra()));
        break;
        case GRAY_SCOTT:
            reaction_system->set_reaction(dynamic_cast<ReactionSystem*>(new ReactionLotkaVolterra()));
        break;
        case BRUSSELATOR:
            reaction_system->set_reaction(dynamic_cast<ReactionSystem*>(new ReactionBrusselator()));
        break;
        default:
            // do nothing
        break;
    }

    reaction_system->set_pbc(this->checkbox_pbc->isChecked());

    reaction_system->set_parameters(this->reaction_settings->get_parameter_string());

    return reaction_system;
}

/**
 * @brief      Build selector widget for reactions
 */
void InputTab::build_reaction_settings(QGridLayout *gridlayout) {
    this->reaction_selector = new QComboBox();

    this->reaction_selector->addItem(tr("Please select a reaction..."));
    this->reaction_selector->addItem(tr("Lotka-Volterra"));
    this->reaction_selector->addItem(tr("Gray-Scott"));
    this->reaction_selector->addItem(tr("Fitzhugh-Nagumo"));
    this->reaction_selector->addItem(tr("Brusselator"));
    this->reaction_selector->addItem(tr("Barkley"));
    this->reaction_selector->setCurrentIndex(0);

    gridlayout->addWidget(new QLabel(tr("<b>Reaction settings</b>")), 0, 0);
    gridlayout->addWidget(this->reaction_selector, 1, 0);
}

/**
 * @brief      Builds general parameters.
 */
void InputTab::build_general_parameters(QGridLayout *gridlayout) {
    this->input_diffusion_X = new QDoubleSpinBox();
    this->input_diffusion_Y = new QDoubleSpinBox();
    this->input_dx = new QDoubleSpinBox();
    this->input_dt = new QDoubleSpinBox();

    unsigned int row = 0;

    gridlayout->addWidget(new QLabel("<b>Integration settings</b>"), row, 0);
    row++;

    gridlayout->addWidget(new QLabel("dX"), row, 0);
    gridlayout->addWidget(this->input_diffusion_X, row, 1);
    this->input_diffusion_X->setDecimals(6);
    this->input_diffusion_X->setValue(2e-5);
    gridlayout->addWidget(new QLabel("Diffusion rate of component X"), row, 2);
    row++;

    gridlayout->addWidget(new QLabel("dY"), row, 0);
    gridlayout->addWidget(this->input_diffusion_Y, row, 1);
    this->input_diffusion_Y->setDecimals(6);
    this->input_diffusion_Y->setValue(1e-5);
    gridlayout->addWidget(new QLabel("Diffusion rate of component Y"), row, 2);
    row++;

    gridlayout->addWidget(new QLabel("dx"), row, 0);
    gridlayout->addWidget(this->input_dx, row, 1);
    this->input_dx->setDecimals(4);
    this->input_dx->setValue(0.005);
    gridlayout->addWidget(new QLabel("Spatial distance in discretization"), row, 2);
    row++;

    gridlayout->addWidget(new QLabel("dt"), row, 0);
    gridlayout->addWidget(this->input_dt, row, 1);
    this->input_dt->setDecimals(4);
    this->input_dt->setValue(0.01);
    gridlayout->addWidget(new QLabel("Time step size"), row, 2);
    row++;

    this->input_width = new QSpinBox();
    this->input_height = new QSpinBox();
    this->input_steps = new QSpinBox();
    this->input_tsteps = new QSpinBox();
    this->input_ncores = new QSpinBox();

    gridlayout->addWidget(new QLabel("width"), row, 0);
    gridlayout->addWidget(this->input_width, row, 1);
    this->input_width->setMinimum(16);
    this->input_width->setMaximum(1024);
    this->input_width->setValue(256);
    gridlayout->addWidget(new QLabel("Width of the unit cell"), row, 2);
    row++;

    gridlayout->addWidget(new QLabel("height"), row, 0);
    gridlayout->addWidget(this->input_height, row, 1);
    this->input_height->setMinimum(16);
    this->input_height->setMaximum(1024);
    this->input_height->setValue(256);
    gridlayout->addWidget(new QLabel("Height of the unit cell"), row, 2);
    row++;

    gridlayout->addWidget(new QLabel("steps"), row, 0);
    gridlayout->addWidget(this->input_steps, row, 1);
    this->input_steps->setMinimum(1);
    this->input_steps->setMaximum(10000);
    this->input_steps->setValue(100);
    gridlayout->addWidget(new QLabel("Number of integration steps"), row, 2);
    row++;

    gridlayout->addWidget(new QLabel("tsteps"), row, 0);
    gridlayout->addWidget(this->input_tsteps, row, 1);
    this->input_tsteps->setMinimum(1);
    this->input_tsteps->setMaximum(10000);
    this->input_tsteps->setValue(1000);
    gridlayout->addWidget(new QLabel("Number of time steps per each integration step"), row, 2);
    row++;

    this->checkbox_pbc = new QCheckBox;
    this->checkbox_pbc->setCheckState(Qt::Checked);
    gridlayout->addWidget(new QLabel("pbc"), row, 0);
    gridlayout->addWidget(this->checkbox_pbc, row, 1);
    gridlayout->addWidget(new QLabel("Whether to implement periodic boundary conditions (checked) or no-flux conditions (unchecked)"), row, 2);
    row++;

    gridlayout->addWidget(new QLabel("ncores"), row, 0);
    gridlayout->addWidget(this->input_ncores, row, 1);
    this->input_ncores->setMinimum(1);
    // determine maximum number of threads; this part needs to be executed in a parallel environment
    #pragma omp parallel
    {
        if(omp_get_thread_num() == 0) {
            this->input_ncores->setValue(std::min(4, omp_get_num_threads()));
            this->input_ncores->setMaximum(omp_get_num_threads());
        }
    }
    gridlayout->addWidget(new QLabel("Number of computing cores in OpenMP parallelization"), row, 2);
    QIcon icon_cores_info = style()->standardIcon(QStyle::SP_MessageBoxWarning);
    QPixmap pixmap_cores_info = icon_cores_info.pixmap(QSize(16, 16));
    QLabel *label_cores_info = new QLabel;
    label_cores_info->setPixmap(pixmap_cores_info);
    label_cores_info->setToolTip("Too high numbers will actually slow down the calculation as the process becomes limited by inter-process communication.");
    gridlayout->addWidget(label_cores_info, row, 3);
    row++;
}

/**
 * @brief      Sets the reaction input widget
 *
 * @param[in]  reactype  The type
 */
void InputTab::set_reaction_input(int reactype) {
    if(this->reaction_settings != nullptr) {
        this->gridlayout_reaction->removeWidget(this->reaction_settings);
        delete this->reaction_settings;
        this->reaction_settings = nullptr;
    }

    switch(reactype) {
        case LOTKA_VOLTERRA:
            this->reaction_settings = new InputLotkaVolterra();
        break;
        case GRAY_SCOTT:
            this->reaction_settings = new InputGrayScott();
        break;
        case BRUSSELATOR:
            this->reaction_settings = new InputBrusselator();
        break;
        default:
            // do nothing
        break;
    }

    if(this->reaction_settings != nullptr) {
        this->gridlayout_reaction->addWidget(this->reaction_settings, 2, 0);
        this->button_submit->setEnabled(true);
        connect(this->reaction_settings->get_button_set_defaults(), SIGNAL (released()), this, SLOT(set_default_values()));
    } else {
        this->button_submit->setEnabled(false);
    }
}

/**
 * @brief      Set default values
 */
void InputTab::set_default_values() {

    QMessageBox msgBox;
    msgBox.setText(tr("You are about to change the integration settings."));
    msgBox.setInformativeText(tr("Are you sure you want to <b>discard</b> the current values and <b>overwrite</b> these with the default values for this kinetic system?"));
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.setIcon(QMessageBox::Warning);
    int ret = msgBox.exec();

    if(ret == QMessageBox::Cancel) {
        return;
    }

    std::string params = this->reaction_settings->get_default_parameter_settings();

    std::vector<std::string> pieces;
    boost::split(pieces, params, boost::is_any_of(";"), boost::token_compress_on);

    for(const std::string& piece : pieces) {
        std::vector<std::string> vars;
        boost::split(vars, piece, boost::is_any_of("="), boost::token_compress_on);

        if(vars.size() != 2) {
            std::cerr << "Piece: " << piece << std::endl;
            std::cerr << "Var size: " << vars.size() << std::endl;
            throw std::runtime_error("Invalid params list encountered: " + params);
        }

        if(vars[0] == "dX") {
            this->input_diffusion_X->setValue(boost::lexical_cast<double>(vars[1]));
            continue;
        }

        if(vars[0] == "dY") {
            this->input_diffusion_Y->setValue(boost::lexical_cast<double>(vars[1]));
            continue;
        }

        if(vars[0] == "dx") {
            this->input_dx->setValue(boost::lexical_cast<double>(vars[1]));
            continue;
        }

        if(vars[0] == "dt") {
            this->input_dt->setValue(boost::lexical_cast<double>(vars[1]));
            continue;
        }

        if(vars[0] == "width") {
            this->input_width->setValue(boost::lexical_cast<unsigned int>(vars[1]));
            continue;
        }

        if(vars[0] == "height") {
            this->input_height->setValue(boost::lexical_cast<unsigned int>(vars[1]));
            continue;
        }

        if(vars[0] == "steps") {
            this->input_steps->setValue(boost::lexical_cast<unsigned int>(vars[1]));
            continue;
        }

        if(vars[0] == "tsteps") {
            this->input_tsteps->setValue(boost::lexical_cast<unsigned int>(vars[1]));
            continue;
        }

        if(vars[0] == "pbc") {
            if(vars[1] == "1") {
                this->checkbox_pbc->setCheckState(Qt::Checked);
            } else {
                this->checkbox_pbc->setCheckState(Qt::Unchecked);
            }
            continue;
        }
    }
}
