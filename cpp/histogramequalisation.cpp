/*
 * This file is part of HarbuzHIST18. 
 * histogramequalisation.cpp
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
#include "histogramequalisation.h"
#include "ui_histogramequalisation.h"

HistogramEqualisation::HistogramEqualisation(QImage *img, QWidget *parent) :
    IToolWidget(parent),
    ui(new Ui::HistogramEqualisation),
    _histogram(std::make_unique<Histogram>(img, this, "Equalized image")),
    _image(img)
{
    ui->setupUi(this);
    ui->histLayout->addWidget(_histogram.get());

    connect(_histogram.get(), &IToolWidget::showStatusMsg, this, &HistogramEqualisation::showStatusMsg);
    connect(this, &HistogramEqualisation::sigSourseChanged, _histogram.get(), &IToolWidget::sourceChanged);

    _methodFunctions = {
        {
            /* Average */
            [this](int left, int right){return (left+right)/2;},
            [this](int i, int j){return newZ[Settings::grayCurrLvl(_image->pixel(i,j))];},
        },
        {
            /* Random */
            [this](int left, int right){return right - left;},
            [this](int i, int j){
                int randP = rand() % newZ[Settings::grayCurrLvl(_image->pixel(i,j))];
                return randP + left[Settings::grayCurrLvl(_image->pixel(i,j))];
            },
        },
        {
            /* Neighbours */
            [this](int, int){return 0;},
            [this](int i, int j){
                int avg = 0;
                int maxi = (i+1)>= _image->width() ? _image->width()-1 : (i+1);
                int maxj = (j+1)>= _image->height() ? _image->height()-1 : (j+1);
                int cnt = 0;
                for (int ii = (i-1)<0?0:(i-1) ; ii <= maxi; ++ii){
                    for (int jj = (j-1)<0?0:(j-1); jj <= maxj; ++jj) {
                        if (ii == i && jj == j) continue;
                        avg += Settings::grayCurrLvl(_image->pixel(i,j));
                        ++cnt;
                    }
                }
                avg /= cnt;
                if (avg > right[Settings::grayCurrLvl(_image->pixel(i,j))])
                    return right[Settings::grayCurrLvl(_image->pixel(i,j))];
                if (avg < left[Settings::grayCurrLvl(_image->pixel(i,j))])
                    return left[Settings::grayCurrLvl(_image->pixel(i,j))];
                return avg;
            },
        },
        {
            /* Min-max */
            [this](int left, int right){return (right + left)/2;},
            [this](int i, int j){
                if (Settings::grayCurrLvl(_image->pixel(i,j)) <= newZ[Settings::grayCurrLvl(_image->pixel(i,j))])
                    return left[Settings::grayCurrLvl(_image->pixel(i,j))];
                return right[Settings::grayCurrLvl(_image->pixel(i,j))];
            },
        }
    };
}

HistogramEqualisation::~HistogramEqualisation()
{
    delete ui;
}

void HistogramEqualisation::on_pushButton_clicked()
{
    int method = Methods::Default;
    if (ui->avg_check->isChecked())
        method = Methods::Average;
    else if (ui->neigh_check->isChecked())
        method = Methods::Neighbours;
    else if (ui->rand_check->isChecked())
        method = Methods::Random;
    else if (ui->minMax_check->isChecked())
        method = Methods::MinMax;

    QImage* res = new QImage();
    *res = _image->convertToFormat(QImage::Format_Grayscale8);
    auto hist = _histogram->histogram();

    int r = 0;
    int hint = 0;

    int havg = std::accumulate(hist.begin(), hist.end(), 0);
    havg /= Settings::maxLevels;

    for(int z(0); z < Settings::maxLevels; ++z) {
        left[z] = r;
        hint += hist[z];
        while (hint > havg) {
            hint -= havg;
            ++r;
        }
        right[z] = r;
        newZ[z] = _methodFunctions[method][0](left[z], right[z]);
    }
    for (int i(0); i < _image->width(); ++i) {
        emit setProgressBar((100./_image->width())*i);
        for (int j(0); j < _image->height(); ++j) {
            int px = 0;
            if (left[Settings::grayCurrLvl(_image->pixel(i,j))] == right[Settings::grayCurrLvl(_image->pixel(i,j))])
                px = Settings::to256gray(right[Settings::grayCurrLvl(_image->pixel(i,j))]);
            else
                px = Settings::to256gray(_methodFunctions[method][1](i,j));
            res->setPixel(i,j, qRgb(px,px,px));
        }
    }

    emit hideProgressBar();
    emit setPreview(res);
    _image = res;
}

void HistogramEqualisation::sourceChanged(QImage *img)
{
    emit sigSourseChanged(img);
}
