/*
 * This file is part of HarbuzHIST18. 
 * histogramequalisation.h
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
#ifndef HISTOGRAMEQUALISATION_H
#define HISTOGRAMEQUALISATION_H

#include <QFrame>
#include "histogram.h"
#include <memory>
#include <functional>
#include <vector>
#include "itoolwidget.h"
#include "settings.h"

namespace Ui {
class HistogramEqualisation;
}

class HistogramEqualisation : public IToolWidget
{
    Q_OBJECT

public:
    explicit HistogramEqualisation(QImage *img, QWidget *parent = 0);
    ~HistogramEqualisation();

private slots:
    void on_pushButton_clicked();

private:
    Ui::HistogramEqualisation *ui;
    std::unique_ptr<Histogram> _histogram;
    QImage* _image;

    enum Methods {Default = 0, Average = 0, Random, Neighbours, MinMax};

    std::vector<std::array<std::function<int(int,int)>,2>> _methodFunctions;

    std::vector<int> left = std::vector<int>(Settings::maxLevels,0);
    std::vector<int> right = std::vector<int>(Settings::maxLevels,0);
    std::vector<int> newZ = std::vector<int>(Settings::maxLevels,0);
};

#endif // HISTOGRAMEQUALISATION_H
