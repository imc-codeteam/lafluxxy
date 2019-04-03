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

#include "worker_thread.h"

WorkerThread::WorkerThread(TwoDimRD* _reaction_system) {
    this->reaction_system = _reaction_system;
    this->continue_running = true;
}

void WorkerThread::run() {
    for(unsigned int i=0; i<this->reaction_system->get_num_steps(); i++) {

        if(this->continue_running == false) {
            emit simulation_cancelled();
            return;
        }

        this->reaction_system->update();

        emit step_finished(i);
    }

    emit simulation_finished();
}
