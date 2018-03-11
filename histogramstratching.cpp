#include "histogramstratching.h"
#include "ui_histogramstratching.h"

HistogramStratching::HistogramStratching(QImage *img, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::HistogramStratching),
    _histogram(std::make_unique<Histogram>(img, this)),
    _image(img)
{
    ui->setupUi(this);
    ui->histLayout->addWidget(_histogram.get());
}

HistogramStratching::~HistogramStratching()
{
    delete ui;
}

void HistogramStratching::on_applyButton_clicked()
{
    auto [low, up] = _histogram->getSelection();

    QImage *res = new QImage();
    *res = _image->convertToFormat(QImage::Format_Grayscale8);

    int px(0);
    for (int i(0); i < res->width(); ++i) {
        for (int j(0); j < res->height(); ++j) {
            px = qGray(res->pixel(i,j));
            if (px > low && px <= up) {
                px = (px-low)*(Histogram::maxLevels / (up-low));
            } else {
                px = 0;
            }
            res->setPixel(i,j, qRgb(px,px,px));
        }
    }

    emit setPreview(res);

    _image = res;
    Histogram *newHist = new Histogram(res, this);
    _histogram.reset(newHist);
    ui->histLayout->addWidget(_histogram.get());
}
