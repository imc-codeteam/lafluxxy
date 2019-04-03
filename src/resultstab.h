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

#ifndef _RESULTSTAB_H
#define _RESULTSTAB_H

#include <QWidget>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QLabel>
#include <QToolButton>
#include <QStyle>
#include <QProgressBar>

#include "renderarea.h"

class ResultsTab : public QWidget {
    Q_OBJECT

private:
    RenderArea *renderarea;
    QToolButton *button_next;
    QToolButton *button_prev;
    QProgressBar *progress_bar;
    QLabel *frame_label;

    TwoDimRD* reaction_system;

public:
    /**
     * @brief Input tab constructor
     * @param parent widget
     */
    explicit ResultsTab(QWidget *parent = 0);

    void update_progress(unsigned int i, unsigned int total);

    inline void set_reaction_system(TwoDimRD* _reaction_system) {
        this->reaction_system = _reaction_system;
    }

    void add_frame(unsigned int i);

private:

private slots:
    void next_img();

    void prev_img();
};

#endif // _RESULTSTAB_H
