#include "histogramequalisation.h"
#include "ui_histogramequalisation.h"

HistogramEqualisation::HistogramEqualisation(QImage *img, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::HistogramEqualisation),
    _histogram(std::make_unique<Histogram>(img, this)),
    _image(img)
{
    ui->setupUi(this);
    ui->histLayout->addWidget(_histogram.get());

    _methodFunctions = {
        {
            /* Average */
            [this](int left, int right){return (left+right)/2;},
            [this](int i, int j){return newZ[qGray(_image->pixel(i,j))];},
        },
        {
            /* Random */
            [this](int left, int right){return right - left;},
            [this](int i, int j){
                int randP = rand() % newZ[qGray(_image->pixel(i,j))];
                return randP + left[qGray(_image->pixel(i,j))];
            },
        },
        {
            /* Neighbours */
            [this](int, int){return 0;},
            [this](int i, int j){
                int avg = 0;
                int maxi = (i+1)>= _image->width() ? _image->width()-1 : (i+1);
                int maxj = (j+1)>= _image->height() ? _image->height()-1 : (j+1);
                int cnt = 0;
                for (int ii = (i-1)<0?0:(i-1) ; ii <= maxi; ++ii){
                    for (int jj = (j-1)<0?0:(j-1); jj <= maxj; ++jj) {
                        if (ii == i && jj == j) continue;
                        avg += qGray(_image->pixel(i,j));
                        ++cnt;
                    }
                }
                avg /= cnt;
                if (avg > right[qGray(_image->pixel(i,j))])
                    return right[qGray(_image->pixel(i,j))];
                if (avg < left[qGray(_image->pixel(i,j))])
                    return left[qGray(_image->pixel(i,j))];
                return avg;
            },
        },
    };
}

HistogramEqualisation::~HistogramEqualisation()
{
    delete ui;
}

void HistogramEqualisation::on_pushButton_clicked()
{
    int method = Methods::Default;
    if (ui->avg_check->isChecked())
        method = Methods::Average;
    else if (ui->neigh_check->isChecked())
        method = Methods::Neighbours;
    else if (ui->rand_check->isChecked())
        method = Methods::Random;

    QImage* res = new QImage();
    *res = _image->convertToFormat(QImage::Format_Grayscale8);
    auto hist = _histogram->histogram();

    int r = 0;
    int hint = 0;

    int havg = std::accumulate(hist.begin(), hist.end(), 0);
    havg /= Histogram::maxLevels;

    for(int z(0); z < Histogram::maxLevels; ++z) {
        left[z] = r;
        hint += hist[z];
        while (hint > havg) {
            hint -= havg;
            ++r;
        }
        right[z] = r;
        newZ[z] = _methodFunctions[method][0](left[z], right[z]);
    }
    for (int i(0); i < _image->width(); ++i) {
        for (int j(0); j < _image->height(); ++j) {
            int px = 0;
            if (left[qGray(_image->pixel(i,j))] == right[qGray(_image->pixel(i,j))])
                px = right[qGray(_image->pixel(i,j))];
            else
                px = _methodFunctions[method][1](i,j);
            res->setPixel(i,j, qRgb(px,px,px));
        }
    }

    emit setPreview(res);

    _image = res;
    Histogram* newHist = new Histogram(res, this);
    _histogram.reset(newHist);
    ui->histLayout->addWidget(_histogram.get());
}
