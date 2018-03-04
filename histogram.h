#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <QtCharts>
#include <QBarSeries>
#include <memory>
#include "callout.h"

class Histogram : public QChartView
{
    Q_OBJECT
public:
    Histogram(QImage& img, QWidget* parent = nullptr);
    virtual ~Histogram(){
        delete m_tooltip;
        delete m_coordX;
        delete m_coordY;
    }

    void mouseMoveEvent(QMouseEvent* event) override;

private:
    long hist[256] = {0};
    QGraphicsSimpleTextItem *m_coordX;
    QGraphicsSimpleTextItem *m_coordY;
    Callout *m_tooltip;
    QList<Callout *> m_callouts;

public slots:
    void keepCallout(int, QBarSet*);
    void tooltip(bool state, int index, QBarSet*bar);
};

#endif // HISTOGRAM_H
