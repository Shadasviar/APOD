#include "histogram.h"

Histogram::Histogram(QImage &img)
{
    for (int i(0); i < img.width(); ++i) {
        for (int j(0); j < img.height(); ++j) {
            ++hist[qGray(img.pixel(i,j))];
        }
    }
    auto* histSet = new QBarSet("Histogram");

    for (auto& lvl : hist) {
        *histSet << lvl;
    }

    auto* barseries = new QBarSeries();
    barseries->append(histSet);

    auto* chart = new QChart();
    barseries->setBarWidth(1);
    chart->addSeries(barseries);

    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    m_tooltip = new Callout(chart);
    m_coordX = new QGraphicsSimpleTextItem();
    m_coordY = new QGraphicsSimpleTextItem();

    connect(barseries, &QBarSeries::clicked, this, &Histogram::keepCallout);
    connect(barseries, &QBarSeries::hovered, this, &Histogram::tooltip);

    this->setMouseTracking(true);
}

void Histogram::mouseMoveEvent(QMouseEvent *event)
{
    m_coordX->setText(QString("X: %1").arg(chartView->chart()->mapToValue(event->pos()).x()));
    m_coordY->setText(QString("Y: %1").arg(chartView->chart()->mapToValue(event->pos()).y()));
    QGraphicsView::mouseMoveEvent(event);
}

void Histogram::keepCallout(int, QBarSet *)
{
    m_callouts.append(m_tooltip);
    m_tooltip = new Callout(chartView->chart());
}

void Histogram::tooltip(bool state, int index, QBarSet *bar)
{
    if (m_tooltip == 0)
        m_tooltip = new Callout(chartView->chart());

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
