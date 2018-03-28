/*
 * This file is part of HarbuzHIST18. 
 * universalpointoperation.h
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
#ifndef UNIVERSALPOINTOPERATION_H
#define UNIVERSALPOINTOPERATION_H

#include <QFrame>
#include "itoolwidget.h"
#include <QtCharts>
#include <QLineSeries>
#include <memory>
#include <set>
#include <array>

namespace Ui {
class UniversalPointOperation;
}

/******** Subclass for ChartView ********************************/
class PlotView : public QChartView
{
    Q_OBJECT
public:
    explicit PlotView(QWidget *parent = 0):QChartView(parent){}
    ~PlotView(){}

    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

signals:
    void mouseMovedTo(QPointF x);
    void mousePressedAt(QPointF x);
};
/******** End of subclass ChartView *****************************/

class UniversalPointOperation : public IToolWidget
{
    Q_OBJECT

public:
    static constexpr int maxLevels = 256;

    explicit UniversalPointOperation(QImage* img, QWidget *parent = 0);
    ~UniversalPointOperation();

    static QImage* applyUPO(const QImage* img, std::array<int, maxLevels> op);

private:
    Ui::UniversalPointOperation *ui;
    QImage* _image;
    std::unique_ptr<QLineSeries> _lineSeries;
    std::unique_ptr<PlotView> _chartView;

    struct QPointFLess {
        bool operator() (const QPointF& a, const QPointF& b){
            return (int)a.x() < (int)b.x();
        }
    };
    std::set<QPointF, QPointFLess> _points;
    std::array<int, maxLevels> _fFromX {{0}};

    void refreshOperationTable();
    int inRange(int x, int l, int r);

protected slots:
    void chartMouseMovedTo(QPointF x);
    void chartMousePressedAt(QPointF x);
};

#endif // UNIVERSALPOINTOPERATION_H
