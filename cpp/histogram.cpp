/*
 * This file is part of HarbuzHIST18. 
 * histogram.cpp
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
#include "histogram.h"
#include "ui_histogram.h"
#include <utility>

Histogram::Histogram(QImage *img, QWidget *parent):
    QFrame(parent),
    _histSet(std::make_unique<QBarSet>("Histogram")),
    _image(img),
    _chartView(std::make_unique<HistView>(this)),
    ui(new Ui::Histogram)
{
    ui->setupUi(this);

    connect(_chartView.get(), &HistView::mouseMovedTo, this, &Histogram::chartMouseMovedTo);
    connect(_chartView.get(), &HistView::mousePressedAt, this, &Histogram::chartMousePressedAt);

    for(int i(0); i < _image->width(); ++i){
        for (int j(0); j < _image->height(); ++j) {
            ++_hist[qGray(_image->pixel(i,j))];
        }
    }

    for (auto& lvl: _hist) {
        *_histSet << lvl;
    }

    auto* barseries = new QBarSeries();
    barseries->append(_histSet.get());
    barseries->setBarWidth(1);

    auto* chart = new QChart();
    chart->legend()->hide();
    chart->addSeries(barseries);
    chart->setContentsMargins(-11,-11,-11,-11);

    _chartView->setChart(chart);
    _chartView->setRenderHint(QPainter::Antialiasing);

    ui->histLayout->addWidget(_chartView.get());
    _chartView->setMouseTracking(true);
}

void Histogram::chartMouseMovedTo(QPointF x)
{
    int x_val = _chartView->chart()->mapToValue(x).x();
    ui->x_value_label->setText(QString("%1").arg(x_val));
    ui->y_value_label->setText(QString("%1").arg(
                                   x_val >= 0 && x_val < maxLevels
                                                     ?(int)_hist[x_val]
                                                      : 0));
}

void Histogram::chartMousePressedAt(QPointF x)
{
    if (_lastSetLower) {
        _upBound = _chartView->chart()->mapToValue(x).x();
    } else {
        _lowBound = _chartView->chart()->mapToValue(x).x();
    }
    _lastSetLower = !_lastSetLower;
    if (_lowBound > _upBound)
        std::swap(_lowBound, _upBound);
    if (_lowBound < 0) _lowBound = 0;
    if (_upBound > maxLevels) _upBound = maxLevels - 1;
}

Histogram::~Histogram()
{
    delete ui;
}

std::pair<int, int> Histogram::getSelection()
{
    return std::make_pair(_lowBound, _upBound);
}

std::array<double, Histogram::maxLevels> Histogram::histogram()
{
    auto res = _hist;
    return res;
}

void HistView::mouseMoveEvent(QMouseEvent *event)
{
    emit mouseMovedTo(event->pos());
}

void HistView::mousePressEvent(QMouseEvent *event)
{
    emit mousePressedAt(event->pos());
    QGraphicsLineItem *line = scene()->addLine(event->pos().x(), 0, event->pos().x(), height());
    _lines.append(line);
    if (_lines.size() > 2) {
        delete _lines.first();
        _lines.pop_front();
    }
}
