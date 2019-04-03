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

#ifndef _RENDER_ARA
#define _RENDER_ARA

#include <QWidget>
#include <QPainter>
#include <QPixmap>

#include "two_dim_rd.h"
#include "reaction_lotka_volterra.h"

class RenderArea : public QWidget {
    Q_OBJECT

private:
    std::shared_ptr<TwoDimRD> rd_system;
    std::vector<QPixmap> graphs;

public:
    /**
     * @brief Input tab constructor
     * @param parent widget
     */
    explicit RenderArea(QWidget *parent = 0);

    QSize minimumSizeHint() const override;

    QSize sizeHint() const override;

protected:
    void paintEvent(QPaintEvent *event) override;

private:

private slots:

};

#endif // _RENDER_ARA
