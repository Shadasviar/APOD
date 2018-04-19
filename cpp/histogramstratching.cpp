/*
 * This file is part of HarbuzHIST18. 
 * histogramstratching.cpp
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
#include "histogramstratching.h"
#include "ui_histogramstratching.h"
#include "settings.h"

HistogramStratching::HistogramStratching(QImage *img, QWidget *parent) :
    IToolWidget(parent),
    ui(new Ui::HistogramStratching),
    _histogram(std::make_unique<Histogram>(img, this)),
    _image(img)
{
    ui->setupUi(this);
    ui->histLayout->addWidget(_histogram.get());
    connect(_histogram.get(), &IToolWidget::showStatusMsg, this, &IToolWidget::showStatusMsg);
}

HistogramStratching::~HistogramStratching()
{
    delete ui;
}

void HistogramStratching::on_applyButton_clicked()
{
    auto low = std::get<0>(_histogram->getSelection());
    auto up = std::get<1>(_histogram->getSelection());

    QImage *res = new QImage();
    *res = _image->convertToFormat(QImage::Format_Grayscale8);

    int px(0);
    for (int i(0); i < res->width(); ++i) {
        emit setProgressBar((100./_image->width())*i);
        for (int j(0); j < res->height(); ++j) {
            px = Settings::grayCurrLvl(res->pixel(i,j));
            if (px > low && px <= up) {
                px = Settings::to256gray((px-low)*(Settings::maxLevels / (up-low)));
            } else {
                px = 0;
            }
            res->setPixel(i,j, qRgb(px,px,px));
        }
    }

    emit hideProgressBar();
    emit setPreview(res);

    _image = res;
    Histogram *newHist = new Histogram(res, this);
    _histogram.reset(newHist);
    ui->histLayout->addWidget(_histogram.get());
    connect(_histogram.get(), &IToolWidget::showStatusMsg, this, &IToolWidget::showStatusMsg);
}
