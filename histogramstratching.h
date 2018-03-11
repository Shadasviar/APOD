#ifndef HISTOGRAMSTRATCHING_H
#define HISTOGRAMSTRATCHING_H

#include <QFrame>
#include <memory>
#include "histogram.h"
#include "itoolwidget.h"

namespace Ui {
class HistogramStratching;
}

class HistogramStratching : public IToolWidget
{
    Q_OBJECT

public:
    explicit HistogramStratching(QImage* img, QWidget *parent = 0);
    ~HistogramStratching();

private slots:
    void on_applyButton_clicked();

private:
    Ui::HistogramStratching *ui;
    std::unique_ptr<Histogram> _histogram;
    QImage* _image;
};

#endif // HISTOGRAMSTRATCHING_H
