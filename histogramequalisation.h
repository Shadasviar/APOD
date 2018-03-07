#ifndef HISTOGRAMEQUALISATION_H
#define HISTOGRAMEQUALISATION_H

#include <QFrame>
#include "histogram.h"
#include <memory>

namespace Ui {
class HistogramEqualisation;
}

class HistogramEqualisation : public QFrame
{
    Q_OBJECT

public:
    explicit HistogramEqualisation(QImage &img, QWidget *parent = 0);
    ~HistogramEqualisation();

private slots:
    void on_pushButton_clicked();

private:
    Ui::HistogramEqualisation *ui;
    std::unique_ptr<Histogram> _histogram;
    QImage& _image;
};

#endif // HISTOGRAMEQUALISATION_H
