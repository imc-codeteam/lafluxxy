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

#include <array>

// add auxiliary cuda functions
#include "check_cuda.h"
#include "cuda_events.h"

// include kernels
#include "kernels/laplacians.h"
#include "kernels/reaction_kinetics.h"
#include "kernels/update.h"

// other includes
#include "rd2d_cuda.h"

RD2D_CUDA::RD2D_CUDA() {

}

/**
 * @brief      Run time-integration on GPU
 */
void RD2D_CUDA::update_step() {
    // std::cout << "Starting time-integration" << std::endl;

    // std::cout << "Setting grids and blocks...           ";
    dim3 gridx(this->my / this->pencils, 1, 1);
    dim3 blockx(this->mx, this->pencils, 1);
    dim3 gridy(this->mx / this->pencils, 1, 1);
    dim3 blocky(this->pencils, this->my, 1);
    unsigned int block = this->mx;
    unsigned int grid = (this->ncells + this->mx - 1) / this->mx;

    unsigned shared_mem_size = 0;
    if(this->zeroflux) {
        shared_mem_size = this->pencils * this->mx * sizeof(float);
    } else {
        shared_mem_size = this->pencils * (this->mx + 2) * sizeof(float);
    }
    // std::cout << donestring << std::endl << std::endl;

    // keep track of time
    cudaEvent_t startEvent, stopEvent;
    cudaEvent_t startEventKernel, stopEventKernel;
    checkCuda( cudaEventCreate(&startEvent) );
    checkCuda( cudaEventCreate(&stopEvent) );
    checkCuda( cudaEventCreate(&startEventKernel) );
    checkCuda( cudaEventCreate(&stopEventKernel) );

    // keep track of time
    this->milliseconds = 0;
    this->laplacian_x_times = 0;
    this->laplacian_y_times = 0;
    this->reaction_times = 0;
    this->update_times = 0;
    this->laplacian_summation_times = 0;

    start_event(&startEvent);

    for(unsigned int i=0; i<this->tsteps; i++) {

        // calculate laplacian for A
        if(this->zeroflux) {
            // x2 derivative
            start_event(&startEventKernel);
            derivative_x2_zeroflux<<<gridx,blockx,shared_mem_size>>>(d_a, d_dx2);
            this->laplacian_x_times += stop_event(&startEventKernel, &stopEventKernel);

            // y2 derivative
            start_event(&startEventKernel);
            derivative_y2_zeroflux<<<gridy,blocky,shared_mem_size>>>(d_a, d_dy2);
            this->laplacian_y_times += stop_event(&startEventKernel, &stopEventKernel);
        } else if(this->has_mask) {
            // x2 derivative
            start_event(&startEventKernel);
            derivative_x2_masked<<<gridx,blockx,shared_mem_size>>>(d_a, d_mask, d_dx2);
            this->laplacian_x_times += stop_event(&startEventKernel, &stopEventKernel);;

            // y2 derivative
            start_event(&startEventKernel);
            derivative_y2_masked<<<gridy,blocky,shared_mem_size>>>(d_a, d_mask, d_dy2);
            this->laplacian_y_times += stop_event(&startEventKernel, &stopEventKernel);;
        } else {
            // x2 derivative
            start_event(&startEventKernel);
            derivative_x2_pbc<<<gridx,blockx,shared_mem_size>>>(d_a, d_dx2);
            this->laplacian_x_times += stop_event(&startEventKernel, &stopEventKernel);;

            // y2 derivative
            start_event(&startEventKernel);
            derivative_y2_pbc<<<gridy,blocky,shared_mem_size>>>(d_a, d_dy2);
            this->laplacian_y_times += stop_event(&startEventKernel, &stopEventKernel);;
        }

        // sum all three derivative components
        start_event(&startEventKernel);
        construct_laplacian_a<<<grid,block>>>(d_da, d_dx2, d_dy2);
        this->laplacian_summation_times += stop_event(&startEventKernel, &stopEventKernel);;

        // calculate laplacian for B
        if(this->zeroflux) {
            // x2 derivative
            start_event(&startEventKernel);
            derivative_x2_zeroflux<<<gridx,blockx,shared_mem_size>>>(d_b, d_dx2);
            this->laplacian_x_times += stop_event(&startEventKernel, &stopEventKernel);;

            // y2 derivative
            start_event(&startEventKernel);
            derivative_y2_zeroflux<<<gridy,blocky,shared_mem_size>>>(d_b, d_dy2);
            this->laplacian_y_times += stop_event(&startEventKernel, &stopEventKernel);;
        } else if(this->has_mask) {
            // x2 derivative
            start_event(&startEventKernel);
            derivative_x2_masked<<<gridx,blockx,shared_mem_size>>>(d_b, d_mask, d_dx2);
            this->laplacian_x_times += stop_event(&startEventKernel, &stopEventKernel);;

            // y2 derivative
            start_event(&startEventKernel);
            derivative_y2_masked<<<gridy,blocky,shared_mem_size>>>(d_b, d_mask, d_dy2);
            this->laplacian_y_times += stop_event(&startEventKernel, &stopEventKernel);;
        } else {
            // x2 derivative
            start_event(&startEventKernel);
            derivative_x2_pbc<<<gridx,blockx,shared_mem_size>>>(d_b, d_dx2);
            this->laplacian_x_times += stop_event(&startEventKernel, &stopEventKernel);;

            // y2 derivative
            start_event(&startEventKernel);
            derivative_y2_pbc<<<gridy,blocky,shared_mem_size>>>(d_b, d_dy2);
            this->laplacian_y_times += stop_event(&startEventKernel, &stopEventKernel);;
        }

        // sum all derivative components
        start_event(&startEventKernel);
        construct_laplacian_b<<<grid,block>>>(d_db, d_dx2, d_dy2);
        this->laplacian_summation_times += stop_event(&startEventKernel, &stopEventKernel);;

        // calculate reaction
        start_event(&startEventKernel);

        // execute reaction kinetics step
        switch(this->reacttype) {
            case KINETICS::GRAY_SCOTT:
                reaction_gray_scott<<<grid,block>>>(d_a, d_b, d_ra, d_rb);
            break;
            case KINETICS::BRUSSELATOR:
                reaction_brusselator<<<grid,block>>>(d_a, d_b, d_ra, d_rb);
            break;
            case KINETICS::BARKLEY:
                reaction_barkley<<<grid,block>>>(d_a, d_b, d_ra, d_rb);
            break;
            case KINETICS::LOTKA_VOLTERRA:
                reaction_lotka_volterra<<<grid,block>>>(d_a, d_b, d_ra, d_rb);
            break;
            case KINETICS::FITZHUGH_NAGUMO:
                reaction_fitzhugh_nagumo<<<grid,block>>>(d_a, d_b, d_ra, d_rb);
            break;
            default:
                throw std::runtime_error("Invalid reaction type");
        }

        this->reaction_times += stop_event(&startEventKernel, &stopEventKernel);;

        // update
        start_event(&startEventKernel);
        update<<<grid,block>>>(d_a, d_b, d_da, d_db, d_ra, d_rb);
        this->update_times += stop_event(&startEventKernel, &stopEventKernel);;;
    }

    // stop timer
    checkCuda( cudaEventRecord(stopEvent, 0) );
    checkCuda( cudaEventSynchronize(stopEvent) );
    checkCuda( cudaEventElapsedTime(&this->milliseconds, startEvent, stopEvent) );

    // copy results back
    int bytes = this->ncells * sizeof(float);
    checkCuda( cudaMemcpy(this->a, this->d_a, bytes, cudaMemcpyDeviceToHost) );
    checkCuda( cudaMemcpy(this->b, this->d_b, bytes, cudaMemcpyDeviceToHost) );

    this->other_times = this->milliseconds - this->laplacian_x_times - this->laplacian_y_times -
                        this->reaction_times - this->update_times - this->laplacian_summation_times;

    // printf("------------------------------------------\n");
    // printf("  Integration time: %12.6f ms\n", this->milliseconds);
    // printf("  Laplacian x:      %12.6f ms\n", this->laplacian_x_times);
    // printf("  Laplacian y:      %12.6f ms\n", this->laplacian_y_times);
    // printf("  Laplacian sum:    %12.6f ms\n", this->laplacian_summation_times);
    // printf("  Reaction:         %12.6f ms\n", this->reaction_times);
    // printf("  Time step:        %12.6f ms\n", this->update_times);
    // printf("  Other:            %12.6f ms\n\n", this->other_times);
    // printf("------------------------------------------\n");
    // printf("\n");

    // clean up
    checkCuda( cudaEventDestroy(startEvent) );
    checkCuda( cudaEventDestroy(stopEvent) );
}

/**
 * @brief      Initialize all variables
 */
void RD2D_CUDA::initialize_variables(const std::vector<float>& _a,
                                     const std::vector<float>& _b,
                                     const std::vector<float>& _mask) {
    // std::cout << "Loading device variables." << std::endl;

    // build initial concentrations
    // std::cout << "Constructing initial concentrations...";
    this->a = new float[this->ncells];
    this->b = new float[this->ncells];
    this->mask = new float[this->ncells];
    memcpy(this->a, &_a[0], sizeof(float) * _a.size());
    memcpy(this->b, &_b[0], sizeof(float) * _b.size());
    memcpy(this->mask, &_mask[0], sizeof(float) * _mask.size());
    // std::cout << donestring << std::endl;

    // allocate size on device
    // std::cout << "Allocating variables on GPU device... ";
    int bytes = this->ncells * sizeof(float);
    checkCuda( cudaMalloc((void**)&this->d_a, bytes) );
    checkCuda( cudaMalloc((void**)&this->d_b, bytes) );
    checkCuda( cudaMalloc((void**)&this->d_mask, bytes) );
    checkCuda( cudaMalloc((void**)&this->d_dx2, bytes) );
    checkCuda( cudaMalloc((void**)&this->d_dy2, bytes) );
    checkCuda( cudaMalloc((void**)&this->d_ra, bytes) );
    checkCuda( cudaMalloc((void**)&this->d_rb, bytes) );
    checkCuda( cudaMalloc((void**)&this->d_da, bytes) );
    checkCuda( cudaMalloc((void**)&this->d_db, bytes) );
    // std::cout << donestring << std::endl;

    // copy data to device
    // std::cout << "Copying data to GPU device...         ";
    checkCuda( cudaMemcpy(this->d_a, this->a, bytes, cudaMemcpyHostToDevice) );
    checkCuda( cudaMemcpy(this->d_b, this->b, bytes, cudaMemcpyHostToDevice) );
    checkCuda( cudaMemcpy(this->d_mask, this->mask, bytes, cudaMemcpyHostToDevice) );
    checkCuda( cudaMemset(this->d_dx2, 0, bytes) );
    checkCuda( cudaMemset(this->d_dy2, 0, bytes) );
    checkCuda( cudaMemset(this->d_ra, 0, bytes) );
    checkCuda( cudaMemset(this->d_rb, 0, bytes) );
    checkCuda( cudaMemset(this->d_da, 0, bytes) );
    checkCuda( cudaMemset(this->d_db, 0, bytes) );
    // std::cout << donestring << std::endl;

    // set constants
    // std::cout << "Setting constant variables on GPU...  ";
    float _diffcon_a = this->Da / (this->dx * this->dx);
    float _diffcon_b = this->Db / (this->dx * this->dx);
    checkCuda( cudaMemcpyToSymbol(d_diffcon_a, &_diffcon_a, sizeof(float)) );
    checkCuda( cudaMemcpyToSymbol(d_diffcon_b, &_diffcon_b, sizeof(float)) );
    checkCuda( cudaMemcpyToSymbol(d_dt, &this->dt, sizeof(float)) );
    checkCuda( cudaMemcpyToSymbol(d_mx, &this->mx, sizeof(unsigned int)) );
    checkCuda( cudaMemcpyToSymbol(d_my, &this->my, sizeof(unsigned int)) );
    checkCuda( cudaMemcpyToSymbol(d_pencils, &this->pencils, sizeof(unsigned int)) );
    checkCuda( cudaMemcpyToSymbol(d_ncells, &this->ncells, sizeof(unsigned int)) );
    checkCuda( cudaMemcpyToSymbol(d_c1, &this->c1, sizeof(float)) );
    checkCuda( cudaMemcpyToSymbol(d_c2, &this->c2, sizeof(float)) );
    checkCuda( cudaMemcpyToSymbol(d_c3, &this->c3, sizeof(float)) );
    checkCuda( cudaMemcpyToSymbol(d_c4, &this->c4, sizeof(float)) );
    // std::cout << donestring << std::endl;

    // std::cout << "All ready for time-integration." << std::endl << std::endl;
}

/**
 * @brief      Clean-up all variables
 */
void RD2D_CUDA::cleanup_variables() {
    // std::cout << "Cleaning Integration variables...     ";
    checkCuda( cudaFree(this->d_a) );
    checkCuda( cudaFree(this->d_b) );
    checkCuda( cudaFree(this->d_mask) );
    checkCuda( cudaFree(this->d_ra) );
    checkCuda( cudaFree(this->d_rb) );
    checkCuda( cudaFree(this->d_da) );
    checkCuda( cudaFree(this->d_db) );
    checkCuda( cudaFree(this->d_dx2) );
    checkCuda( cudaFree(this->d_dy2) );

    delete [] this->a;
    delete [] this->b;
    delete [] this->mask;

    // std::cout << donestring << std::endl;
    // std::cout << std::endl;
}
