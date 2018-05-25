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
#include <QtDataVisualization/q3dtheme.h>

Histogram2D::Histogram2D(QImage *img1, QImage *img2, QWidget *parent) :
    IToolWidget(parent),
    ui(new Ui::Histogram2D),
    _img1(img1),
    _img2(std::make_unique<QImage>()),
    _series(new QtDataVisualization::QBar3DSeries)
{
    ui->setupUi(this);
    *_img2 = img2->scaled(_img1->size());

    /* Set 3D plot properties */

    auto *bars = new QtDataVisualization::Q3DBars();
    _series->setItemLabelFormat(
                QStringLiteral("@valueLabel pixels goes from @rowIdx to @colIdx"));
    bars->addSeries(_series);

    auto *container = QWidget::createWindowContainer(bars);


    auto* firstImgAxis = new QtDataVisualization::QCategory3DAxis;
    firstImgAxis->setTitle("Original");
    firstImgAxis->setTitleVisible(true);
    firstImgAxis->setLabelAutoRotation(30.0f);
    firstImgAxis->setLabels(QStringList("Original 0"));

    auto* secImgAxis = new QtDataVisualization::QCategory3DAxis;
    secImgAxis->setTitle("Processed");
    secImgAxis->setTitleVisible(true);
    secImgAxis->setLabelAutoRotation(30.0f);
    secImgAxis->setLabels(QStringList("Processed 0"));

    auto* valAxis = new QtDataVisualization::QValue3DAxis;
    valAxis->setTitle("Moved pixels");
    valAxis->setTitleVisible(true);
    valAxis->setLabelAutoRotation(30.0f);
    valAxis->setLabelFormat(QString("%d"));

    bars->setRowAxis(firstImgAxis);
    bars->setColumnAxis(secImgAxis);
    bars->setValueAxis(valAxis);

    bars->setShadowQuality(QtDataVisualization::QAbstract3DGraph::ShadowQualitySoftMedium);
    bars->activeTheme()->setBackgroundEnabled(true);
    bars->setReflection(true);

    bars->activeTheme()->setType(QtDataVisualization::Q3DTheme::ThemeUserDefined);
    bars->activeTheme()->setBaseColors(QList<QColor>{QColor("steelblue")});
    bars->activeTheme()->setSingleHighlightColor(QColor(100,250,250));

    ui->histlayout->addWidget(container);
    ui->tableWidget->viewport()->installEventFilter(this);

    update();
}

Histogram2D::~Histogram2D()
{
    delete ui;
}

void Histogram2D::sourceChanged(QImage *img)
{
    *_img2 = img->scaled(_img1->size());

    update();
}

void Histogram2D::update()
{
    maxLevel = Settings::maxLevels;

    _histTable = std::vector<std::vector<int>>(maxLevel, std::vector<int>(maxLevel, 0));
    for (int i(0); i < _img1->width(); ++i) {
        for (int j(0); j < _img1->height(); ++j) {
            ++_histTable[Settings::grayCurrLvl(_img1->pixel(i,j))]
                    [Settings::grayCurrLvl(_img2->pixel(i,j))];
        }
    }

    auto *data = new QtDataVisualization::QBarDataArray;
    data->reserve(_histTable.size());
    QtDataVisualization::QBarDataRow *row;

    for (size_t i(0); i < _histTable.size(); ++i) {
        row = new QtDataVisualization::QBarDataRow(_histTable[i].size());
        for (size_t j(0); j < _histTable[i].size(); ++j) {
            (*row)[j].setValue(_histTable[i][j]);
        }
        data->append(row);
    }
    _series->dataProxy()->resetArray(data);

    /* 2D table update */
    ui->tableWidget->setRowCount(maxLevel);
    ui->tableWidget->setColumnCount(maxLevel);
    QStringList hLabels, vLabels;
    for (size_t i(0); i < _histTable.size(); ++i) {
        hLabels << QString::number(i);
        for (size_t j(0); j < _histTable[i].size(); ++j) {
            vLabels << QString::number(j);
            ui->tableWidget->setItem(i, j, new QTableWidgetItem(QString::number(_histTable[j][i])));
            ui->tableWidget->setColumnWidth(i, 55);
        }
    }
    ui->tableWidget->setHorizontalHeaderLabels(hLabels);
    ui->tableWidget->setVerticalHeaderLabels(vLabels);

}

bool Histogram2D::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->tableWidget->viewport()
            && event->type() == QEvent::Drop) {
        /* source rows */
        if (ui->tableWidget->selectedRanges().size() <= 0) return false;
        auto selected = ui->tableWidget->selectedRanges()[0];
        auto* dropEvent = (QDropEvent*)event;
        /* destination row */
        if (!ui->tableWidget->itemAt(dropEvent->pos()))
            return false;
        auto dropped = ui->tableWidget->itemAt(dropEvent->pos())->row();

        QImage* res = new QImage();
        *res = _img1->convertToFormat(QImage::Format_Grayscale8);

        for (int i(0); i < _img1->width(); ++i) {
            emit setProgressBar((100./res->width())*i);
            for (int j(0); j < _img1->height(); ++j) {
                auto px = Settings::grayCurrLvl(_img2->pixel(i,j));
                if (px <= selected.bottomRow()
                        && px >= selected.topRow()) {
                    auto dpx = Settings::to256gray(dropped);
                    res->setPixel(i,j, qRgb(dpx, dpx, dpx));
                } else {
                    res->setPixel(i, j, _img2->pixel(i,j));
                }
            }
        }

        emit hideProgressBar();
        emit setPreview(res);
    }
    return false;
}

