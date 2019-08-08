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

#include <iostream>
#include <fstream>
#include <memory>
#include <vector>

#include "matrices.h"
#include "reaction_system.h"
#include "reaction_gray_scott.h"
#include "rd2d_cuda.h"

class TwoDimRD {
private:
    double Da;              //!< Diffusion coefficient of compound A
    double Db;              //!< Diffusion coefficient of compound B

    unsigned int width;     //!< width of the system
    unsigned int height;    //!< height of the system
    double dx;              //!< size of the space interval
    double dt;              //!< size of the time interval
    unsigned int steps;     //!< number of frames
    unsigned int tsteps;    //!< number of time steps when to write a frame

    MatrixXXd a;            //!< matrix to hold concentration of A
    MatrixXXd b;            //!< matrix to hold concentration of B
    MatrixXXd delta_a;      //!< matrix to store temporary A increment
    MatrixXXd delta_b;      //!< matrix to store temporary B increment

    std::vector<MatrixXXd> ta;  //!< matrix to hold temporal data
    std::vector<MatrixXXd> tb;  //!< matrix to hold temporal data

    double t;   //!< Total time t

    std::unique_ptr<ReactionSystem> reaction_system;    //!< Pointer to reaction system
    std::unique_ptr<RD2D_CUDA> cuda_integrator;         //!< Pointer to reaction system

    bool pbc = true;    //!< Whether to employ periodic boundary conditions
    bool mask = false;  //!< Whether to employ a diffusivity mask (internal no-flux walls)

    MatrixXXi matmask;          //!< Matrix to store the mask

    unsigned int ncores;
    bool do_cuda = false;

public:
    /**
     * @brief      Constructs the object.
     *
     * @param[in]  _Da      Diffusion coefficient of compound A
     * @param[in]  _Db      Diffusion coefficient of compound B
     * @param[in]  _width   width of the system
     * @param[in]  _height  height of the system
     * @param[in]  _dx      size of the space interval
     * @param[in]  _dt      size of the time interval
     * @param[in]  _steps   number of frames
     * @param[in]  _tsteps  number of time steps when to write a frame
     */
    TwoDimRD(double _Da, double _Db,
             unsigned int _width, unsigned int _height,
             double _dx, double _dt, unsigned int _steps, unsigned int _tsteps);

    /**
     * @brief      Sets the reaction.
     *
     * @param      _reaction_system  The reaction system
     */
    void set_reaction(ReactionSystem* _reaction_system);

    /**
     * @brief      Set whether to do time-integration using CUDA
     *
     * @param[in]  _do_cuda  whether to do time-integration using CUDA
     */
    inline void set_do_cuda(bool _do_cuda) {
        this->do_cuda = _do_cuda;
    }

    /**
     * @brief      Set whether system has periodic boundary conditions
     *
     * @param[in]  _pbc  Periodic boundary conditions
     */
    inline void set_pbc(bool _pbc) {
        this->pbc = _pbc;
    }

    /**
     * @brief      Perform time integration
     */
    void time_integrate();

    /**
     * @brief      Perform a time-step
     */
    void update();

    /**
     * @brief      Write the current state of compound A to the file
     *
     * @param[in]  filename  The filename
     */
    void write_state_to_file(const std::string& filename);

    /**
     * @brief      Sets the parameters.
     *
     * @param[in]  params  The parameters
     */
    inline void set_parameters(const std::string& params) {
        this->reaction_system->set_parameters(params);
        this->init();
    }

    inline size_t get_num_img() const {
        return this->ta.size();
    }

    inline unsigned int get_num_steps() const {
        return this->steps;
    }

    /**
     * @brief      Gets the concentration matrix.
     *
     * @param[in]  frame  Frame index
     * @param[in]  first  Whether first or second concentration (X or Y) needs to be returned
     *
     * @return     The concentration matrix.
     */
    const MatrixXXd& get_concentration_matrix(unsigned int frame, bool first) const;

    /**
     * @brief      Gets the concentrations.
     *
     * @param[in]  first  Whether to return concentrations of X
     *
     * @return     The concentrations.
     */
    inline const auto& get_concentrations(bool first) const {
        return first ? this->ta : this->tb;
    }

    /**
     * @brief      Set the number of cores
     *
     * @param[in]  _ncores  Number of cores
     */
    inline void set_cores(unsigned int _ncores) {
        this->ncores = _ncores;
    }

    /**
     * @brief      Set the diffusivity mask
     *
     * @param[in]  _mask  The mask
     */
    inline void set_mask(const MatrixXXi& _mask) {
        this->matmask = _mask;
        this->mask = true;
    }

    /**
     * @brief      Gets the mask.
     *
     * @return     The mask.
     */
    inline const auto& get_mask() const {
        return this->matmask;
    }

    /**
     * @brief      Clean-up any variables that need to be explicitly removed from memory
     */
    void clean();

private:
    /**
     * @brief      Initialize the system
     */
    void init();

    /**
     * @brief      Calculate Laplacian using central finite difference with periodic boundary conditions
     *
     * @param      delta_c  Concentration update matrix
     * @param      c        Current concentration matrix
     *
     * Note that this overwrites the current delta matrices!
     */
    void laplacian_2d_pbc(MatrixXXd& delta_c, const MatrixXXd& c);

    /**
     * @brief      Calculate Laplacian using central finite difference with zero-flux boundaries
     *
     * @param      delta_c  Concentration update matrix
     * @param      c        Current concentration matrix
     *
     * Note that this overwrites the current delta matrices!
     */
    void laplacian_2d_zeroflux(MatrixXXd& delta_c, const MatrixXXd& c);

    /**
     * @brief      Calculate Laplacian using central finite difference with zero-flux mask
     *
     * @param      delta_c  Concentration update matrix
     * @param      c        Current concentration matrix
     *
     * Note that this overwrites the current delta matrices!
     */
    void laplacian_2d_mask(MatrixXXd& delta_c, const MatrixXXd& c);

    /*
     * CACHE OPTIMIZED FUNCTIONS
     */

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
    void laplacian_2d_pbc_cached(MatrixXXd& delta_c, const MatrixXXd& c);

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
    void laplacian_2d_zeroflux_cached(MatrixXXd& delta_c, const MatrixXXd& c);

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
    void laplacian_2d_mask_cached(MatrixXXd& delta_c, const MatrixXXd& c);

    /**
     * @brief      Calculate reaction term
     *
     * Add the value to the current delta matrices
     */
    void add_reaction();

    /**
     * @brief      Apply mask to concentrations
     */
    void apply_mask();

    /**
     * @brief      Special instructions for cuda variant of time update
     */
    void update_cuda();

    /**
     * @brief      Special instructions for cuda variant of initialization
     */
    void init_cuda();
};
