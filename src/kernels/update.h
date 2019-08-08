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

#ifndef _UPDATE_H
#define _UPDATE_H

// constant variables on device
__device__ __constant__ float d_dt;

/**
 * @brief      Perform time-step integration
 *
 * @param      x     pointer to concentration of A
 * @param      y     pointer to concentration of B
 * @param[in]  ddx   diffusion of component A
 * @param[in]  ddy   diffusion of component B
 * @param[in]  drx   reaction of component A
 * @param[in]  dry   reaction of component B
 */
__global__ void update(float *x, float *y, const float *ddx, const float *ddy, const float *drx, const float *dry) {
    int index = blockIdx.x * blockDim.x + threadIdx.x;
    int stride = blockDim.x * gridDim.x;

    for(int i = index; i < d_ncells; i += stride) {
        x[i] += (ddx[i] + drx[i]) * d_dt;
        y[i] += (ddy[i] + dry[i]) * d_dt;
    }
}

/**
 * @brief      Perform time-step integration
 *
 * @param      x     pointer to concentration of A
 * @param      y     pointer to concentration of B
 * @param      mask  maze mask
 * @param[in]  ddx   diffusion of component A
 * @param[in]  ddy   diffusion of component B
 * @param[in]  drx   reaction of component A
 * @param[in]  dry   reaction of component B
 */
__global__ void update_mask(float *x, float *y, float *mask, const float *ddx, const float *ddy, const float *drx, const float *dry) {
    int index = blockIdx.x * blockDim.x + threadIdx.x;
    int stride = blockDim.x * gridDim.x;

    for(int i = index; i < d_ncells; i += stride) {
        if(mask[i] > 0.5f) {
            x[i] = 0.0;
            y[i] = 0.0;
        } else {
            x[i] += (ddx[i] + drx[i]) * d_dt;
            y[i] += (ddy[i] + dry[i]) * d_dt;
        }
    }
}

#endif // _UPDATE_H
