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

#include "two_dim_rd.h"

/**
 * @brief      Constructs the object.
 *
 * @param[in]  _Da      Diffusion coefficient of compound A
 * @param[in]  _Db      Diffusion coefficient of compound B
 * @param[in]  _alpha   Alpha value in reaction equation
 * @param[in]  _beta    Beta value in reaction equation
 * @param[in]  _width   width of the system
 * @param[in]  _height  height of the system
 * @param[in]  _dx      size of the space interval
 * @param[in]  _dt      size of the time interval
 * @param[in]  _steps   number of frames
 * @param[in]  _tsteps  number of time steps when to write a frame
 */
TwoDimRD::TwoDimRD(double _Da, double _Db,
                   unsigned int _width, unsigned int _height,
                   double _dx, double _dt, unsigned int _steps, unsigned int _tsteps) :
    Da(_Da),
    Db(_Db),
    width(_width),
    height(_height),
    dx(_dx),
    dt(_dt),
    steps(_steps),
    tsteps(_tsteps) {

}

void TwoDimRD::set_reaction(ReactionSystem* _reaction_system) {
    this->reaction_system = std::unique_ptr<ReactionSystem>(_reaction_system);
}

/**
 * @brief      Perform time integration
 */
void TwoDimRD::time_integrate() {
    this->t = 0;

    for(unsigned int i=0; i<this->steps; i++) {
        this->update();
    }

    // give newline after tqdm progress bar
    std::cout << std::endl;
}

/**
 * @brief      Write the current state of compound A to the file
 *
 * @param[in]  filename  The filename
 */
void TwoDimRD::write_state_to_file(const std::string& filename) {
    std::ofstream out(filename, std::ios::out | std::ios::binary | std::ios::trunc);

    typename MatrixXXd::Index rows, cols;

    // store width and height
    out.write((char*) (&this->width), sizeof(unsigned int) );
    out.write((char*) (&this->height), sizeof(unsigned int) );

    // store number of frames
    out.write((char*) (&this->steps), sizeof(unsigned int) );

    for(unsigned int i=0; i<this->ta.size(); i++) {
        const auto& a = this->ta[i];
        const auto& b = this->tb[i];

        // store a
        rows = a.rows();
        cols = a.cols();

        out.write((char*) a.data(), rows * cols * sizeof(typename MatrixXXd::Scalar) );

        // store b
        rows = b.rows();
        cols = b.cols();

        out.write((char*) b.data(), rows * cols * sizeof(typename MatrixXXd::Scalar) );
    }

    out.close();
}

/**
 * @brief      Gets the concentration matrix.
 *
 * @param[in]  frame  Frame index
 * @param[in]  first  Whether first or second concentration (X or Y) needs to be returned
 *
 * @return     The concentration matrix.
 */
const MatrixXXd& TwoDimRD::get_concentration_matrix(unsigned int frame, bool first) const {
    if(frame >= this->ta.size()) {
        throw std::runtime_error("Invalid time frame requested");
    } else {
        if(first) {
            return this->ta[frame];
        } else {
            return this->tb[frame];
        }
    }
}

/**
 * @brief      Initialize the system
 */
void TwoDimRD::init() {
    // initialize matrices with random values
    this->a = MatrixXXd::Zero(this->width, this->height);
    this->b = MatrixXXd::Zero(this->width, this->height);

    this->reaction_system->init(this->a, this->b);

    if(this->mask) {
        this->apply_mask();
    }

    this->ta.push_back(this->a);
    this->tb.push_back(this->b);

    if(do_cuda) {
        this->init_cuda();
    } else {
        this->delta_a = MatrixXXd::Zero(this->width, this->height);
        this->delta_b = MatrixXXd::Zero(this->width, this->height);
    }
}

/**
 * @brief      Perform a time-step
 */
void TwoDimRD::update() {
    if(do_cuda) {
        this->update_cuda();
        this->t += this->tsteps * this->dt;
    } else {
        // loop over number of time steps
        for(unsigned int j=0; j<this->tsteps; j++) {

            // calculate laplacian
            if(this->mask) {
                this->laplacian_2d_mask_cached(this->delta_a, this->a);
                this->laplacian_2d_mask_cached(this->delta_b, this->b);
            } else if(this->pbc) {
                this->laplacian_2d_pbc_cached(this->delta_a, this->a);
                this->laplacian_2d_pbc_cached(this->delta_b, this->b);
            } else {
                this->laplacian_2d_zeroflux_cached(this->delta_a, this->a);
                this->laplacian_2d_zeroflux_cached(this->delta_b, this->b);
            }

            // multiply with diffusion coefficient
            this->delta_a *= this->Da;
            this->delta_b *= this->Db;

            // add reaction term
            this->add_reaction();

            // multiply with time step
            this->delta_a *= this->dt;
            this->delta_b *= this->dt;

            // add delta term to concentrations
            this->a += this->delta_a;
            this->b += this->delta_b;

            // apply mask
            if(this->mask) {
                this->apply_mask();
            }

            // update time step
            this->t += this->dt;
        }
    }

    this->ta.push_back(this->a);
    this->tb.push_back(this->b);
}

/**
 * @brief      Calculate Laplacian using central finite difference with periodic boundary conditions
 *
 * @param      delta_c  Concentration update matrix
 * @param      c        Current concentration matrix
 *
 * Note that this overwrites the current delta matrices!
 */
void TwoDimRD::laplacian_2d_pbc(MatrixXXd& delta_c, const MatrixXXd& c) {
    const double idx2 = 1.0 / (this->dx * this->dx);

    omp_set_num_threads(this->ncores);
    #pragma omp parallel for schedule(static)
    for(int i=0; i<(int)this->height; i++) {
        // indices
        unsigned i1 = 0;
        unsigned i2 = 0;

        if(i == 0) {
            i1 = this->height-1;
            i2 = i+1;
        } else if(i == ((int)this->height-1)) {
            i1 = i-1;
            i2 = 0;
        } else {
            i1 = i-1;
            i2 = i+1;
        }

        // loop over x axis
        for(unsigned int j=0; j<this->width; j++) {
            // indices
            unsigned j1 = 0;
            unsigned j2 = 0;

            if(j == 0) {
                j1 = this->width-1;
                j2 = j+1;
            } else if(j == (this->width-1)) {
                j1 = j-1;
                j2 = 0;
            } else {
                j1 = j-1;
                j2 = j+1;
            }

            // calculate laplacian
            delta_c(i,j) = (-4.0 * c(i,j)
                                 + c(i1,j)
                                 + c(i2,j)
                                 + c(i,j1)
                                 + c(i,j2) ) * idx2;
        }
    }
}

/**
 * @brief      Calculate Laplacian using central finite difference with periodic boundary conditions
 *
 * This function attempt to reduce cache misses
 *
 * @param      delta_c  Concentration update matrix
 * @param      c        Current concentration matrix
 *
 * Note that this overwrites the current delta matrices!
 */
void TwoDimRD::laplacian_2d_pbc_cached(MatrixXXd& delta_c, const MatrixXXd& c) {
    const double idx2 = 1.0 / (this->dx * this->dx);

    omp_set_num_threads(this->ncores);
    #pragma omp parallel for schedule(static)
    for(int i=0; i<(int)this->height; i++) {
        // indices
        unsigned i1 = 0;
        unsigned i2 = 0;

        if(i == 0) {
            i1 = this->height-1;
            i2 = i+1;
        } else if(i == ((int)this->height-1)) {
            i1 = i-1;
            i2 = 0;
        } else {
            i1 = i-1;
            i2 = i+1;
        }

        const auto row_up = c.row(i1);
        const auto row_cur = c.row(i);
        const auto row_down = c.row(i2);

        // loop over x axis
        for(unsigned int j=0; j<this->width; j++) {
            // indices
            unsigned j1 = 0;
            unsigned j2 = 0;

            if(j == 0) {
                j1 = this->width-1;
                j2 = j+1;
            } else if(j == (this->width-1)) {
                j1 = j-1;
                j2 = 0;
            } else {
                j1 = j-1;
                j2 = j+1;
            }

            // calculate laplacian
            delta_c(i,j) = (-4.0 * row_cur(j)
                                 + row_down(j)
                                 + row_up(j)
                                 + row_cur(j1)
                                 + row_cur(j2) ) * idx2;
        }
    }
}

/**
 * @brief      Calculate Laplacian using central finite difference with zero-flux boundaries
 *
 * @param      delta_c  Concentration update matrix
 * @param      c        Current concentration matrix
 *
 * Note that this overwrites the current delta matrices!
 */
void TwoDimRD::laplacian_2d_zeroflux(MatrixXXd& delta_c, const MatrixXXd& c) {
    unsigned int mheight = c.rows();
    unsigned int mwidth = c.cols();

    const double idx2 = 1.0 / (dx * dx);

    omp_set_num_threads(this->ncores);
    #pragma omp parallel for
    for(int i=0; i<(int)mheight; i++) {
        for(unsigned int j=0; j<mwidth; j++) {

            double ddx = 0;
            double ddy = 0;

            if(i == 0) {
                ddy = c(i+1,j) - c(i, j);
            } else if(i == ((int)mheight - 1)) {
                ddy = c(i-1,j) - c(i, j);
            } else {
                ddy = (-2.0 * c(i,j) + c(i-1,j) + c(i+1,j));
            }

            if(j == 0) {
                ddx = c(i,j+1) - c(i, j);
            } else if(j == (mwidth - 1)) {
                ddx = c(i,j-1) - c(i, j);
            } else {
                ddx = (-2.0 * c(i,j) + c(i,j-1) + c(i,j+1));
            }

            // calculate laplacian
            delta_c(i,j) = (ddx + ddy) * idx2;
        }
    }
}

/**
 * @brief      Calculate Laplacian using central finite difference with zero-flux boundaries
 *
 * This function attempt to reduce cache misses
 *
 * @param      delta_c  Concentration update matrix
 * @param      c        Current concentration matrix
 *
 * Note that this overwrites the current delta matrices!
 */
void TwoDimRD::laplacian_2d_zeroflux_cached(MatrixXXd& delta_c, const MatrixXXd& c) {
    unsigned int mheight = c.rows();
    unsigned int mwidth = c.cols();

    const double idx2 = 1.0 / (dx * dx);

    omp_set_num_threads(this->ncores);
    #pragma omp parallel for
    for(int i=0; i<(int)mheight; i++) {

        const auto& row_down = c.row(i > 0 ? i-1 : i);
        const auto& row_cur = c.row(i);
        const auto& row_up = c.row(i < ((int)mheight-1) ? i+1 : i);

        for(unsigned int j=0; j<mwidth; j++) {

            double ddx = 0;
            double ddy = 0;

            if(i == 0) {
                ddy = row_up(j) - row_cur(j);
            } else if(i == ((int)mheight - 1)) {
                ddy = row_down(j) - row_cur(j);
            } else {
                ddy = (-2.0 * row_cur(j) + row_down(j) + row_up(j));
            }

            if(j == 0) {
                ddx = c(i,j+1) - c(i, j);
            } else if(j == (mwidth - 1)) {
                ddx = c(i,j-1) - c(i, j);
            } else {
                ddx = (-2.0 * row_cur(j) + row_cur(j-1) + row_cur(j+1));
            }

            // calculate laplacian
            delta_c(i,j) = (ddx + ddy) * idx2;
        }
    }
}

/**
 * @brief      Calculate Laplacian using central finite difference with zero-flux mask
 *
 * @param      delta_c  Concentration update matrix
 * @param      c        Current concentration matrix
 *
 * Note that this overwrites the current delta matrices!
 */
void TwoDimRD::laplacian_2d_mask(MatrixXXd& delta_c, const MatrixXXd& c) {
    unsigned int mheight = c.rows();
    unsigned int mwidth = c.cols();

    const double idx2 = 1.0 / (dx * dx);

    omp_set_num_threads(this->ncores);
    #pragma omp parallel for
    for(int i=0; i<(int)mheight; i++) {
        for(unsigned int j=0; j<mwidth; j++) {

            if(this->matmask(i,j) == 1) {
                continue;
            }

            double ddx = 0;
            double ddy = 0;

            if(this->matmask(i-1,j) == 1) {         // north boundary
                ddy = c(i+1,j) - c(i, j);
            } else if(this->matmask(i+1,j) == 1) {  // south boundary
                ddy = c(i-1,j) - c(i, j);
            } else {                                // otherwise
                ddy = (-2.0 * c(i,j) + c(i-1,j) + c(i+1,j));
            }

            if(this->matmask(i,j-1) == 1) {         // west boundary
                ddx = c(i,j+1) - c(i, j);
            } else if(this->matmask(i,j+1) == 1) {  // east boundary
                ddx = c(i,j-1) - c(i, j);
            } else {
                ddx = (-2.0 * c(i,j) + c(i,j-1) + c(i,j+1));
            }

            // calculate laplacian
            delta_c(i,j) = (ddx + ddy) * idx2;
        }
    }
}

/**
 * @brief      Calculate Laplacian using central finite difference with zero-flux mask
 *
 * This function attempt to reduce cache misses
 *
 * @param      delta_c  Concentration update matrix
 * @param      c        Current concentration matrix
 *
 * Note that this overwrites the current delta matrices!
 */
void TwoDimRD::laplacian_2d_mask_cached(MatrixXXd& delta_c, const MatrixXXd& c) {
    unsigned int mheight = c.rows();
    unsigned int mwidth = c.cols();

    const double idx2 = 1.0 / (dx * dx);

    omp_set_num_threads(this->ncores);
    #pragma omp parallel for
    for(int i=0; i<(int)mheight; i++) {

        const auto& row_down = c.row(i > 0 ? i-1 : i);
        const auto& row_cur = c.row(i);
        const auto& row_up = c.row(i < ((int)mheight-1) ? i+1 : i);

        const auto& mask_down = this->matmask.row(i > 0 ? i-1 : i);
        const auto& mask_cur = this->matmask.row(i);
        const auto& mask_up = this->matmask.row(i < ((int)mheight-1) ? i+1 : i);

        for(unsigned int j=0; j<mwidth; j++) {

            if(mask_cur(j) == 1) {
                continue;
            }

            double ddx = 0;
            double ddy = 0;

            if(mask_up(j) == 1) {                   // north boundary
                ddy = row_down(j) - row_cur(j);
            } else if(mask_down(j) == 1) {          // south boundary
                ddy = row_up(j) - row_cur(j);
            } else {                                // otherwise
                ddy = (-2.0 * row_cur(j) + row_down(j) + row_up(j));
            }

            if(mask_cur(j-1) == 1) {                // west boundary
                ddx = row_cur(j+1) - row_cur(j);
            } else if(mask_cur(j+1) == 1) {         // east boundary
                ddx = row_cur(j-1) - row_cur(j);
            } else {                                // otherwise
                ddx = (-2.0 * row_cur(j) + row_cur(j-1) + row_cur(j+1));
            }

            // calculate laplacian
            delta_c(i,j) = (ddx + ddy) * idx2;
        }
    }
}

/**
 * @brief      Calculate reaction term
 *
 * Add the value to the current delta matrices
 */
void TwoDimRD::add_reaction() {
    omp_set_num_threads(this->ncores);

    #pragma omp parallel for schedule(static)
    for(int i=0; i<(int)this->height; i++) {
        for(unsigned int j=0; j<this->width; j++) {

            const double a = this->a(i,j);
            const double b = this->b(i,j);

            double ra = 0;
            double rb = 0;

            this->reaction_system->reaction(a, b, &ra, &rb);

            this->delta_a(i,j) += ra;
            this->delta_b(i,j) += rb;
        }
    }
}

/**
 * @brief      Calculate reaction term
 *
 * Add the value to the current delta matrices
 */
void TwoDimRD::apply_mask() {
    omp_set_num_threads(this->ncores);
    #pragma omp parallel for schedule(static)
    for(int i=0; i<(int)this->height; i++) {
        for(unsigned int j=0; j<this->width; j++) {
            if(this->matmask(i,j) == 1) {
                this->a(i,j) = 0.0;
                this->b(i,j) = 0.0;
            }
        }
    }
}

/**
 * @brief      Special instructions for cuda variant
 */
void TwoDimRD::update_cuda() {
    // perform step
    this->cuda_integrator->update_step();

    // copy results
    unsigned int ncells = width * height;
    const float* _a = this->cuda_integrator->get_a();
    const float* _b = this->cuda_integrator->get_b();

    for(unsigned int i=0; i<ncells; i++) {
        *(this->a.data() + i) = (double)*(_a + i);
    }

    for(unsigned int i=0; i<ncells; i++) {
        *(this->b.data() + i) = (double)*(_b + i);
    }

}

/**
 * @brief      Special instructions for cuda variant of initialization
 */
void TwoDimRD::init_cuda() {
    // build object
    this->cuda_integrator = std::make_unique<RD2D_CUDA>();

    // initialize values
    unsigned int ncells = width * height;
    std::vector<float> values_a(ncells);
    std::vector<float> values_b(ncells);
    for(unsigned int i=0; i<ncells; i++) {
        values_a[i] = (float)*(this->a.data() + i);
    }

    for(unsigned int i=0; i<ncells; i++) {
        values_b[i] = (float)*(this->b.data() + i);
    }

    this->cuda_integrator->initialize_variables(values_a, values_b);
}
