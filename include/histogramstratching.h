/*
 * This file is part of HarbuzHIST18. 
 * histogramstratching.h
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
#ifndef HISTOGRAMSTRATCHING_H
#define HISTOGRAMSTRATCHING_H

#include <QFrame>
#include <memory>
#include "histogram.h"
#include "itoolwidget.h"

namespace Ui {
class HistogramStratching;
}

class HistogramStratching : public IToolWidget
{
    Q_OBJECT

public:
    explicit HistogramStratching(QImage* img, QWidget *parent = 0);
    ~HistogramStratching();

private slots:
    void on_applyButton_clicked();

private:
    Ui::HistogramStratching *ui;
    std::unique_ptr<Histogram> _histogram;
    QImage* _image;
};

#endif // HISTOGRAMSTRATCHING_H
