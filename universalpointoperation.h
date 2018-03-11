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
            if ((int)a.x() != (int)b.x())
                return (int)a.x() < (int)b.x();
            return (int)a.y() < (int)b.y();
        }
    };
    std::set<QPointF, QPointFLess> _points;
    std::array<int, maxLevels> _fFromX = {0};

    void refreshOperationTable();
    int inRange(int x, int l, int r);

protected slots:
    void chartMouseMovedTo(QPointF x);
    void chartMousePressedAt(QPointF x);
};

#endif // UNIVERSALPOINTOPERATION_H
