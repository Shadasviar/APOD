#ifndef HISTOGRAMEQUALISATION_H
#define HISTOGRAMEQUALISATION_H

#include <QFrame>
#include "histogram.h"
#include <memory>
#include <functional>
#include <vector>
#include "itoolwidget.h"

namespace Ui {
class HistogramEqualisation;
}

class HistogramEqualisation : public IToolWidget
{
    Q_OBJECT

public:
    explicit HistogramEqualisation(QImage *img, QWidget *parent = 0);
    ~HistogramEqualisation();

private slots:
    void on_pushButton_clicked();

private:
    Ui::HistogramEqualisation *ui;
    std::unique_ptr<Histogram> _histogram;
    QImage* _image;

    enum Methods {Default = 0, Average = 0, Random, Neighbours, MinMax};

    std::vector<std::array<std::function<int(int,int)>,2>> _methodFunctions;

    int left[Histogram::maxLevels] {0};
    int right[Histogram::maxLevels] {0};
    int newZ[Histogram::maxLevels] {0};
};

#endif // HISTOGRAMEQUALISATION_H
