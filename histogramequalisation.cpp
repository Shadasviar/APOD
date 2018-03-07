#include "histogramequalisation.h"
#include "ui_histogramequalisation.h"

HistogramEqualisation::HistogramEqualisation(QImage &img, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::HistogramEqualisation),
    _histogram(std::make_unique<Histogram>(img, this)),
    _image(img)
{
    ui->setupUi(this);
    ui->histLayout->addWidget(_histogram.get());
}

HistogramEqualisation::~HistogramEqualisation()
{
    delete ui;
}

void HistogramEqualisation::on_pushButton_clicked()
{

}
