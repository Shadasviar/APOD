#include "histogram.h"

Histogram::Histogram(QImage &img, QWidget *parent) : QChartView(parent)
{
    for (int i(0); i < img.width(); ++i) {
        for (int j(0); j < img.height(); ++j) {
            ++hist[qGray(img.pixel(i,j))];
        }
    }
    _histSet = new QBarSet("Histogram");

    for (auto& lvl : hist) {
        *_histSet << lvl;
    }

    auto* barseries = new QBarSeries();
    barseries->append(_histSet);

    auto* chart = new QChart();
    barseries->setBarWidth(1);
    chart->addSeries(barseries);

    setChart(chart);
    setRenderHint(QPainter::Antialiasing);

    m_tooltip = new Callout(chart);
    m_coordX = new QGraphicsSimpleTextItem();
    m_coordY = new QGraphicsSimpleTextItem();

    connect(barseries, &QBarSeries::clicked, this, &Histogram::keepCallout);
    connect(barseries, &QBarSeries::hovered, this, &Histogram::tooltip);

    this->setMouseTracking(true);
}

QPointF Histogram::getXSelection()
{
    QPoint res = {0,0};

    return res;
}

void Histogram::mouseMoveEvent(QMouseEvent *event)
{
    m_coordX->setText(QString("X: %1").arg(chart()->mapToValue(event->pos()).x()));
    m_coordY->setText(QString("Y: %1").arg(chart()->mapToValue(event->pos()).y()));
    QGraphicsView::mouseMoveEvent(event);
}

void Histogram::mousePressEvent(QMouseEvent *event)
{
    _lowBound.setX(event->pos().x());
    _lowBound.setY(this->height() - 50);
}

void Histogram::keepCallout(int, QBarSet *)
{
    m_callouts.append(m_tooltip);
    m_tooltip = new Callout(chart());
}

void Histogram::tooltip(bool state, int index, QBarSet *bar)
{
    if (m_tooltip == 0)
        m_tooltip = new Callout(chart());

    if (state) {
        m_tooltip->setText(QString("X: %1 \nY: %2 ").arg(index).arg(bar->at(index)));
        m_tooltip->setAnchor(QPointF(index, bar->at(index)));
        m_tooltip->setZValue(11);
        m_tooltip->updateGeometry();
        m_tooltip->show();
    } else {
        m_tooltip->hide();
    }
}
