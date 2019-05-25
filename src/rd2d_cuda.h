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

#ifndef _RD2D_CUDA_H
#define _RD2D_CUDA_H

#include <iostream>
#include <stdio.h>
#include <assert.h>
#include <random>
#include <fstream>
#include <chrono>

#include "config.h"

class RD2D_CUDA {
private:
    unsigned int mx = 0;
    unsigned int my = 0;
    unsigned int ncells = mx * my;

    unsigned int pencils = 2;

    float *a;       //!< concentration of component A
    float *b;       //!< concentration of component B
    float *mask;    //!< maze mask

    // device variables
    float *d_a, *d_b, *d_mask, *d_dx2, *d_dy2, *d_ra, *d_rb, *d_da, *d_db;

    // reaction settings of kinetic system
    KINETICS reacttype;         //!< type of the kinetic system
    float Da = 0.16;            //!< diffusion constant of A
    float Db = 0.08;            //!< diffusion constant of B
    float dt = 0.25;            //!< temporal discretization
    float dx = 0.5;             //!< spatial discretization

    // generalized reaction parameters
    float c1 = 0.0f;
    float c2 = 0.0f;
    float c3 = 0.0f;
    float c4 = 0.0f;

    unsigned int timesteps = 720;
    unsigned int tsteps = 100;

    std::string donestring = "           [DONE]";

    bool zeroflux = true;
    bool has_mask = false;

    // time tracking
    float milliseconds = 0;
    float laplacian_x_times = 0;
    float laplacian_y_times = 0;
    float reaction_times = 0;
    float update_times = 0;
    float laplacian_summation_times = 0;
    float other_times = 0;

public:
    /**
     * @brief      Constructs the object.
     */
    RD2D_CUDA();

    /**
     * @brief      Get the A concentrations
     *
     * @return     A concentrations
     */
    inline const float* get_a() const {
        return this->a;
    }

    /**
     * @brief      Get the B concentrations
     *
     * @return     B concentrations
     */
    inline const float* get_b() const {
        return this->b;
    }

    /**
     * @brief      Initialize all variables
     */
    void initialize_variables(const std::vector<float>& _a,
                              const std::vector<float>& _b,
                              const std::vector<float>& _mask);

    /**
     * @brief      Run time-integration on GPU
     */
    void update_step();

    /**
     * @brief      Set unit cell dimensions
     *
     * @param[in]  _mx   dimensionality for x
     * @param[in]  _my   dimensionality for y
     */
    inline void set_dimensions(unsigned int _mx, unsigned int _my) {
        this->mx = _mx;
        this->my = _my;
        this->ncells = this->mx * this->my;
    }

    /**
     * @brief      Sets the integration variables.
     *
     * @param[in]  _dt         Time discretization
     * @param[in]  _dx         Distance discretization
     * @param[in]  _timesteps  Number of output timesteps
     * @param[in]  _tsteps     Number of internal timesteps
     */
    inline void set_integration_variables(float _dt, float _dx, unsigned int _timesteps, unsigned int _tsteps) {
        this->dt = _dt;
        this->dx = _dx;
        this->timesteps = _timesteps;
        this->tsteps = _tsteps;
    }

    /**
     * @brief      Sets the kinetic variables.
     *
     * @param[in]  c     Generalized kinetic parameters
     */
    inline void set_kinetic_variables(const std::array<double, 4>& c) {
        this->c1 = c[0];
        this->c2 = c[1];
        this->c3 = c[2];
        this->c4 = c[3];
    }

    /**
     * @brief      Sets the kinetic variables.
     *
     * @param[in]  _f    Gray-Scott parameter f
     * @param[in]  _k    Gray-Scott parameter k
     */
    inline void set_diffusion_parameters(double _Da, double _Db) {
        this->Da = _Da;
        this->Db = _Db;
    }

    /**
     * @brief      Set zeroflux boundary conditions
     *
     * @param[in]  _zeroflux  The zeroflux boundary condition
     */
    inline void set_zeroflux(bool _zeroflux) {
        this->zeroflux = _zeroflux;
    }

    /**
     * @brief      Set whether there is a mask in the spatial domain
     *
     * @param[in]  _mask  The mask setting
     */
    inline void set_mask(bool _mask) {
        this->has_mask = _mask;
    }

    /**
     * @brief      Set the kinetic system
     *
     * @param[in]  _reacttype  The kinetic system
     */
    inline void set_reacttype(KINETICS _reacttype) {
        this->reacttype = _reacttype;
    }

    /**
     * @brief      Clean-up all variables
     */
    void cleanup_variables();

private:
};


#endif // _RD2D_CUDA_H
