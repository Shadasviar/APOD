/*
 * This file is part of HarbuzHIST18. 
 * universalpointoperation.cpp
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
#include "universalpointoperation.h"
#include "ui_universalpointoperation.h"

UniversalPointOperation::UniversalPointOperation(QImage *img, QWidget *parent) :
    IToolWidget(parent),
    ui(new Ui::UniversalPointOperation),
    _image(img),
    _lineSeries(std::make_unique<QLineSeries>()),
    _chartView(std::make_unique<PlotView>(this))
{
    ui->setupUi(this);

    connect(_chartView.get(), &PlotView::mouseMovedTo, this, &UniversalPointOperation::chartMouseMovedTo);
    connect(_chartView.get(), &PlotView::mousePressedAt, this, &UniversalPointOperation::chartMousePressedAt);

    for (int i(0); i < maxLevels; ++i)
        _fFromX[i] = i;

    _points.insert({0,0});
    _points.insert({maxLevels-1, maxLevels-1});

    for (auto& point: _points)
        _lineSeries->append(point);

    QChart* chart = new QChart();
    chart->legend()->hide();
    chart->addSeries(_lineSeries.get());
    chart->createDefaultAxes();
    chart->setContentsMargins(-11,-11,-11,-11);

    _chartView->setChart(chart);
    _chartView->setRenderHint(QPainter::Antialiasing);

    ui->plotLayout->addWidget(_chartView.get());
}

UniversalPointOperation::~UniversalPointOperation()
{
    delete ui;
}

QImage *UniversalPointOperation::applyUPO(const QImage *img, std::array<int, UniversalPointOperation::maxLevels> op)
{
    QImage* res = new QImage();
    *res = img->convertToFormat(QImage::Format_Grayscale8);

    int px = 0;
    for (int i(0); i < res->width(); ++i) {
        for (int j(0); j < res->height(); ++j) {
            px = op[qGray(img->pixel(i,j))];
            res->setPixel(i, j, qRgb(px,px,px));
        }
    }

    return res;
}

void UniversalPointOperation::refreshOperationTable()
{
    auto a = [](QPointF p1, QPointF p2) -> double {
        return (p2.y() - p1.y())/(p2.x() - p1.x());
    };

    auto b = [&a](QPointF p1, QPointF p2) -> double {
        return p2.y() - p2.x() * a(p1, p2);
    };

    QPointF prePoint(0,0);
    for (auto& point : _points){
        if (point == QPointF(0,0)) {
            prePoint = point;
            continue;
        }

        for (int x(prePoint.x()); x <= point.x(); ++x) {
            _fFromX[x] = (int)(a(prePoint, point) * x + b(prePoint, point));
        }
        prePoint = point;
    }
}

int UniversalPointOperation::inRange(int x, int l, int r)
{
    x = x < l ? l : x;
    x = x >= r ? r-1 : x;
    return x;
}

void UniversalPointOperation::chartMouseMovedTo(QPointF x)
{
    int x_val = inRange(_chartView->chart()->mapToValue(x).x(),
                        0, maxLevels);
    ui->x_label->setText(QString("X: %1").arg(x_val));
    ui->y_label->setText(QString("f(X): %1").arg(_fFromX[x_val]));
}

void UniversalPointOperation::chartMousePressedAt(QPointF x)
{
    int x_val = inRange(_chartView->chart()->mapToValue(x).x(),
                        0, maxLevels);

    int y_val = inRange(_chartView->chart()->mapToValue(x).y(),
                         0, maxLevels);

    if(_points.find(QPointF(x_val, y_val)) != _points.end())
        _points.erase(QPointF(x_val, y_val));

    _points.insert(QPointF(x_val, y_val));
    _lineSeries->clear();
    for (auto& point : _points)
        _lineSeries->append(point);

    refreshOperationTable();

    emit setPreview(applyUPO(_image, _fFromX));
}


void PlotView::mouseMoveEvent(QMouseEvent *event)
{
    emit mouseMovedTo(event->pos());
}

void PlotView::mousePressEvent(QMouseEvent *event)
{
    emit mousePressedAt(event->pos());
}
