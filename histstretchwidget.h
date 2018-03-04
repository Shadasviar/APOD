#ifndef HISTSTRETCHWIDGET_H
#define HISTSTRETCHWIDGET_H

#include "histogram.h"
#include <memory>

class HistStretchWidget : public Histogram
{
    Q_OBJECT

public:
    HistStretchWidget(QImage& image, QWidget* parent = nullptr);
    ~HistStretchWidget() {
        delete _leftArrow;
        delete _rightArrow;
    }
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    int mapToHistVal(QPointF pos);
    void strentchSelected();

signals:
    void setPreview(QImage* img);

protected:
    std::unique_ptr<QImage> _image;

private:
    QPointF _lowBound;
    QPointF _upBound;

    QGraphicsPixmapItem* _leftArrow = nullptr;
    QGraphicsPixmapItem* _rightArrow = nullptr;
};

#endif // HISTSTRETCHWIDGET_H
