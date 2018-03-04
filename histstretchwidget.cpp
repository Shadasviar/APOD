#include "histstretchwidget.h"

HistStretchWidget::HistStretchWidget(QImage &image, QWidget *parent)
    :Histogram(image, parent),
      _image(nullptr)
{
    _image = std::make_unique<QImage>(image.copy());
}

void HistStretchWidget::mousePressEvent(QMouseEvent *event)
{
    _lowBound.setX(event->pos().x());
    _lowBound.setY(this->height() - 50);
    delete _leftArrow;
    delete _rightArrow;
}

void HistStretchWidget::mouseReleaseEvent(QMouseEvent *event)
{
    _upBound.setX(event->pos().x());
    _upBound.setY(this->height() - 50);
    if (_lowBound.x() > _upBound.x()) std::swap(_lowBound, _upBound);
    _lowBound.setX(_lowBound.x() - 25);

    _rightArrow = new QGraphicsPixmapItem();
    _rightArrow->setPos(_upBound);
    _rightArrow->setPixmap(QPixmap(":/Resources/right_arrow.png").scaled(30,30));
    this->scene()->addItem(_rightArrow);

    _leftArrow = new QGraphicsPixmapItem();
    _leftArrow->setPos(_lowBound);
    _leftArrow->setPixmap(QPixmap(":/Resources/left_arrow.png").scaled(30,30));
    this->scene()->addItem(_leftArrow);

    strentchSelected();
}

int HistStretchWidget::mapToHistVal(QPointF pos)
{
    auto scenePos = mapToScene(pos.toPoint());
    auto chartPos = this->chart()->mapFromScene(scenePos);
    return this->chart()->mapToValue(chartPos).x();
}

void HistStretchWidget::strentchSelected()
{
    QImage* res = new QImage();
    *res = _image->convertToFormat(QImage::Format_Grayscale8);

    int lower = mapToHistVal(_lowBound);
    int upper = mapToHistVal(_upBound);
    int px = 0;
    for (int i(0); i < res->width(); ++i) {
        for (int j(0); j < res->height(); ++j) {
            px = qGray(res->pixel(i,j));
            if (px > lower && px <= upper) {
                px = (px - lower) * (_histMax/(upper - lower));
            } else {
                px = 0;
            }
            res->setPixel(i,j,qRgb(px,px,px));
        }
    }
    emit setPreview(res);

    for(auto&i : hist) i = 0;
    for (int i(0); i < res->width(); ++i) {
        for (int j(0); j < res->height(); ++j) {
            ++hist[qGray(res->pixel(i,j))];
        }
    }

    _histSet->remove(0, _histMax);
    for (int i(0); i < _histMax; ++i) {
        *_histSet << hist[i];
    }
}
