#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <QFrame>
#include <QtCharts>
#include <QBarSeries>
#include <memory>

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

class Histogram : public QFrame
{
    Q_OBJECT

public:
    explicit Histogram(QImage* img, QWidget *parent = 0);
    ~Histogram();

    std::tuple<int,int> getSelection();

    static constexpr int maxLevels=256;

protected:
    double _hist[maxLevels] {0};
    std::unique_ptr<QBarSet> _histSet;
    QImage* _image;
    std::unique_ptr<HistView> _chartView;

    int _lowBound {0};
    int _upBound {maxLevels-1};

protected slots:
    void chartMouseMovedTo(QPointF x);
    void chartMousePressedAt(QPointF x);

private:
    Ui::Histogram *ui;
    bool _lastSetLower = false;
};

#endif // HISTOGRAM_H
