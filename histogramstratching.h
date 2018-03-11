#ifndef HISTOGRAMSTRATCHING_H
#define HISTOGRAMSTRATCHING_H

#include <QFrame>
#include <memory>
#include "histogram.h"

namespace Ui {
class HistogramStratching;
}

class HistogramStratching : public QFrame
{
    Q_OBJECT

public:
    explicit HistogramStratching(QImage* img, QWidget *parent = 0);
    ~HistogramStratching();

signals:
    void setPreview(QImage*);

private slots:
    void on_applyButton_clicked();

private:
    Ui::HistogramStratching *ui;
    std::unique_ptr<Histogram> _histogram;
    QImage* _image;
};

#endif // HISTOGRAMSTRATCHING_H
