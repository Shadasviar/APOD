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

signals:
    void mouseMovedTo(QPointF x);
};
/*****************************************************************/

class Histogram : public QFrame
{
    Q_OBJECT

public:
    explicit Histogram(QImage* img, QWidget *parent = 0);
    ~Histogram();
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    static constexpr int maxLevels=256;

protected:
    double _hist[maxLevels] {0};
    std::unique_ptr<QBarSet> _histSet;
    QImage* _image;
    std::unique_ptr<HistView> _chartView;

    std::unique_ptr<QGraphicsRectItem> _selectionRectangle;
    QPoint _selectionBegin = {0,0};
    bool _mousePressed = false;

protected slots:
    void chartMouseMovedTo(QPointF x);

private:
    Ui::Histogram *ui;
};

#endif // HISTOGRAM_H
