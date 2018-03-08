#include "histogram.h"
#include "ui_histogram.h"

Histogram::Histogram(QImage *img, QWidget *parent):
    QFrame(parent),
    _histSet(std::make_unique<QBarSet>("Histogram")),
    _image(img),
    _chartView(std::make_unique<HistView>(parent)),
    _selectionRectangle(std::make_unique<QGraphicsRectItem>(0,0,0,0)),
    ui(new Ui::Histogram)
{
    ui->setupUi(this);

    connect(_chartView.get(), &HistView::mouseMovedTo, this, &Histogram::chartMouseMovedTo);

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
    chart->addSeries(barseries);

    _chartView->setChart(chart);
    _chartView->setRenderHint(QPainter::Antialiasing);

    ui->histLayout->addWidget(_chartView.get());
    _chartView->setMouseTracking(true);

    _chartView->scene()->addItem(_selectionRectangle.get());
    this->setMouseTracking(true);
}

void Histogram::chartMouseMovedTo(QPointF x)
{
    int x_val = _chartView->chart()->mapToValue(x).x();
    ui->x_value_label->setText(QString("%1").arg(x_val));
    ui->y_value_label->setText(QString("%1").arg(
                                   x_val >= 0 && x_val < maxLevels
                                                     ?_hist[x_val]
                                                     : 0));
}

Histogram::~Histogram()
{
    delete ui;
}

void Histogram::mousePressEvent(QMouseEvent *event)
{
    _selectionRectangle->setRect(_chartView->chart()->mapToScene(
                                     _chartView->mapFromParent(event->pos())).x(),
                                 0, 1, _chartView->height());
    _selectionBegin = {event->pos()};
    _mousePressed = true;
}

void Histogram::mouseMoveEvent(QMouseEvent *event)
{
    if (_mousePressed) {
        _selectionRectangle->setRect(
            _selectionBegin.x(),
            0,
            _chartView->chart()->mapToScene(_chartView->mapFromParent(event->pos())).x() -
                    _chartView->chart()->mapToScene(_chartView->mapFromParent(_selectionBegin)).x(),
            _chartView->height());
    }
}

void Histogram::mouseReleaseEvent(QMouseEvent *event)
{
    _mousePressed = false;
}

void HistView::mouseMoveEvent(QMouseEvent *event)
{
    emit mouseMovedTo(event->pos());
}
