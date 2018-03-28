/*
 * This file is part of HarbuzHIST18.
 * histogram2d.cpp
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

#include "histogram2d.h"
#include "ui_histogram2d.h"
#include <Q3DBars>
#include <QtDataVisualization>

Histogram2D::Histogram2D(QImage *img1, QImage *img2, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::Histogram2D),
    _img1(img1),
    _img2(std::make_unique<QImage>())
{
    ui->setupUi(this);
    *_img2 = img2->scaled(_img1->size());

    for (int i(0); i < _img1->width(); ++i) {
        for (int j(0); j < _img1->height(); ++j) {
            ++_histTable[qGray(_img1->pixel(i,j))]
                    [qGray(_img2->pixel(i,j))];
        }
    }

    auto *bars = new QtDataVisualization::Q3DBars();
    auto *series = new QtDataVisualization::QBar3DSeries;
    bars->addSeries(series);

    auto *container = QWidget::createWindowContainer(bars);

    auto *data = new QtDataVisualization::QBarDataArray;
    QtDataVisualization::QBarDataRow *row;

    auto* firstImgAxis = new QtDataVisualization::QCategory3DAxis;
    firstImgAxis->setTitle("First image");
    firstImgAxis->setTitleVisible(true);
    firstImgAxis->setLabelAutoRotation(30.0f);
    firstImgAxis->setLabels(QStringList("First 0"));

    auto* secImgAxis = new QtDataVisualization::QCategory3DAxis;
    secImgAxis->setTitle("Second image");
    secImgAxis->setTitleVisible(true);
    secImgAxis->setLabelAutoRotation(30.0f);
    secImgAxis->setLabels(QStringList("Second 0"));

    bars->setRowAxis(firstImgAxis);
    bars->setColumnAxis(secImgAxis);

    bars->setShadowQuality(QtDataVisualization::QAbstract3DGraph::ShadowQualitySoftMedium);
    bars->activeTheme()->setBackgroundEnabled(false);

    data->reserve(_histTable.size());
    for (size_t i(0); i < _histTable.size(); ++i) {
        row = new QtDataVisualization::QBarDataRow(_histTable[i].size());
        for (size_t j(0); j < _histTable[i].size(); ++j) {
            (*row)[j].setValue(_histTable[i][j]);
        }
        data->append(row);
    }
    series->dataProxy()->resetArray(data);

    ui->histlayout->addWidget(container);
}

Histogram2D::~Histogram2D()
{
    delete ui;
}
