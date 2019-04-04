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

#ifndef _WORKER_THREAD_H
#define _WORKER_THREAD_H

#include <QThread>
#include <chrono>

#include "two_dim_rd.h"

class WorkerThread : public QThread {
    Q_OBJECT

private:
    TwoDimRD* reaction_system;
    bool continue_running;

public:
    WorkerThread(TwoDimRD* _reaction_system);

    void run() override;

signals:
    void simulation_finished();

    void simulation_cancelled();

    void step_finished(unsigned int i, double tcalc);

public slots:
    void kill_job() {
        this->continue_running = false;
    }
};

#endif // _WORKER_THREAD_H
