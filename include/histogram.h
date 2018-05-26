/*
 * This file is part of HarbuzHIST18. 
 * histogram.h
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
#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <QFrame>
#include <QtCharts>
#include <QBarSeries>
#include <memory>
#include <vector>
#include "itoolwidget.h"
#include "settings.h"

namespace Ui {
class Histogram;
}

/****** HistView small subclass *********************************/
class HistView : public QChartView {
    Q_OBJECT
public:
    explicit HistView(QWidget *parent = 0):QChartView(parent){}
    ~HistView(){}

    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

signals:
    void mouseMovedTo(QPointF x);
    void mousePressedAt(QPointF x);

private:
    QList<QGraphicsLineItem*> _lines;
};
/*****************************************************************/

class Histogram : public IToolWidget
{
    Q_OBJECT

public:
    explicit Histogram(QImage* img, QWidget *parent = 0, QString title = "");
    ~Histogram();

    int maxLevels=Settings::maxLevels;

    std::pair<int,int> getSelection();
    std::vector<double> histogram();

protected:
    std::vector<double> _hist = std::vector<double>(maxLevels, 0);
    QImage* _image;
    std::unique_ptr<HistView> _chartView;

    int _lowBound {0};
    int _upBound {maxLevels-1};

protected slots:
    void chartMouseMovedTo(QPointF x);
    void chartMousePressedAt(QPointF x);
    void sourceChanged(QImage* img);

private:
    Ui::Histogram *ui;
    bool _lastSetLower = false;
    QString _title = "";
    int _n_cols = 0;
    void init();
};

#endif // HISTOGRAM_H
