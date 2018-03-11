#include "universalpointoperation.h"
#include "ui_universalpointoperation.h"

UniversalPointOperation::UniversalPointOperation(QImage *img, QWidget *parent) :
    IToolWidget(parent),
    ui(new Ui::UniversalPointOperation),
    _image(img),
    _lineSeries(std::make_unique<QLineSeries>()),
    _chartView(std::make_unique<PlotView>(this))
{
    ui->setupUi(this);

    connect(_chartView.get(), &PlotView::mouseMovedTo, this, &UniversalPointOperation::chartMouseMovedTo);
    connect(_chartView.get(), &PlotView::mousePressedAt, this, &UniversalPointOperation::chartMousePressedAt);

    for (int i(0); i < maxLevels; ++i)
        _fFromX[i] = i;

    _points.insert({0,0});
    _points.insert({maxLevels-1, maxLevels-1});

    for (auto& point: _points)
        _lineSeries->append(point);

    QChart* chart = new QChart();
    chart->legend()->hide();
    chart->addSeries(_lineSeries.get());
    chart->createDefaultAxes();
    chart->setContentsMargins(-11,-11,-11,-11);

    _chartView->setChart(chart);
    _chartView->setRenderHint(QPainter::Antialiasing);

    ui->plotLayout->addWidget(_chartView.get());
}

UniversalPointOperation::~UniversalPointOperation()
{
    delete ui;
}

void UniversalPointOperation::refreshOperationTable()
{
    auto a = [](QPointF p1, QPointF p2) -> double {
        return (p2.y() - p1.y())/(p2.x() - p1.x());
    };

    auto b = [&a](QPointF p1, QPointF p2) -> double {
        return p2.y() - p2.x() * a(p1, p2);
    };

    QPointF prePoint(0,0);
    for (auto& point : _points){
        if (point == QPointF(0,0)) {
            prePoint = point;
            continue;
        }

        for (int x(prePoint.x()); x < point.x(); ++x) {
            _fFromX[x] = (int)(a(prePoint, point) * x + b(prePoint, point));
        }
        prePoint = point;
    }
}

int UniversalPointOperation::inRange(int x, int l, int r)
{
    x = x < l ? l : x;
    x = x >= r ? r-1 : x;
    return x;
}

void UniversalPointOperation::chartMouseMovedTo(QPointF x)
{
    int x_val = inRange(_chartView->chart()->mapToValue(x).x(),
                        0, maxLevels);
    ui->x_label->setText(QString("X: %1").arg(x_val));
    ui->y_label->setText(QString("f(X): %1").arg(_fFromX[x_val]));
}

void UniversalPointOperation::chartMousePressedAt(QPointF x)
{
    int x_val = inRange(_chartView->chart()->mapToValue(x).x(),
                        0, maxLevels);

    int y_val = inRange(_chartView->chart()->mapToValue(x).y(),
                         0, maxLevels);

    _points.insert({x_val, y_val});
    _lineSeries->clear();
    for (auto& point : _points)
        _lineSeries->append(point);

    refreshOperationTable();
}


void PlotView::mouseMoveEvent(QMouseEvent *event)
{
    emit mouseMovedTo(event->pos());
}

void PlotView::mousePressEvent(QMouseEvent *event)
{
    emit mousePressedAt(event->pos());
}
