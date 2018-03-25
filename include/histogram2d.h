/*
 * This file is part of HarbuzHIST18.
 * histogram2d.h
 * Copyright (C) Uladzislau Harbuz 2018
 *
 * HarbuzHIST18 is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * HarbuzHIST18 is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef HISTOGRAM2D_H
#define HISTOGRAM2D_H

#include <QFrame>

namespace Ui {
class Histogram2D;
}

class Histogram2D : public QFrame
{
    Q_OBJECT

public:
    explicit Histogram2D(QImage* img1, QImage* img2, QWidget *parent = 0);
    ~Histogram2D();

private:
    Ui::Histogram2D *ui;
};

#endif // HISTOGRAM2D_H
