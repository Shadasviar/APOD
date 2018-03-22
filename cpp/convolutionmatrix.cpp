#include "convolutionmatrix.h"
#include "ui_convolutionmatrix.h"
#include "histogram.h"

QMap<QString, std::pair<int, int>> ConvolutionMatrix::_allowedMatrixSizes = {
    {"3x3", {3,3}},
    {"3x5", {3,5}},
    {"5x3", {5,3}},
    {"5x5", {5,5}},
    {"7x7", {7,7}},
};

QMap<ConvolutionMatrix::BorderMethods, ConvolutionMatrix::borderFunction>
ConvolutionMatrix::_borderMethods = {

    {ConvolutionMatrix::NoChange, [](const QImage* img, int i, int j, int,
        std::vector<std::vector<int> >) {
            return qGray(img->pixel(i,j));
     }},

    {ConvolutionMatrix::Multiply, [](const QImage* img, int i, int j, int divisor,
        std::vector<std::vector<int> > mask) {
            int avg(0);
            for (int ii(i-mask.size()/2); ii < (int)(i+mask.size()/2); ++ii) {
                for (int jj(j-mask[0].size()/2); jj < (int)(j+mask[0].size()/2); ++jj) {
                    if (ii < 0 || ii >= img->width()) {
                        if (jj >= 0 && jj < img->height())
                            avg += 2*qGray(img->pixel(i, jj))
                                *mask[(ii-i)+mask.size()/2][(jj-j)+mask[0].size()/2];
                    }
                    else if (jj < 0 || jj >= img->height()) {
                        avg += 2*qGray(img->pixel(ii,j))
                            *mask[(ii-i)+mask.size()/2][(jj-j)+mask[0].size()/2];
                    }
                    else
                        avg += qGray(img->pixel(ii,jj))
                            *mask[(ii-i)+mask.size()/2][(jj-j)+mask[0].size()/2];
                }
            }
            return avg / divisor;
     }},

    {ConvolutionMatrix::ExistingOnly, [](const QImage* img, int i, int j, int divisor,
        std::vector<std::vector<int> > mask) {
         int avg(0);
         for (int ii(i-mask.size()/2); ii < (int)(i+mask.size()/2); ++ii) {
             for (int jj(j-mask[0].size()/2); jj < (int)(j+mask[0].size()/2); ++jj) {
                 if (ii < 0 || ii >= img->width() || jj < 0 || jj >= img->height()) {
                     --divisor;
                     if (divisor <= 0) divisor = 1;
                 }
                 else {
                     avg += qGray(img->pixel(ii,jj))
                         *mask[(ii-i)+mask.size()/2][(jj-j)+mask[0].size()/2];
                 }
             }
         }
         return avg / divisor;
     }},
};

ConvolutionMatrix::ConvolutionMatrix(QImage* img, QWidget *parent) :
    IToolWidget(parent),
    ui(new Ui::ConvolutionMatrix),
    _image(img)
{
    ui->setupUi(this);
    emit on_maskSizeBox_currentTextChanged(ui->maskSizeBox->currentText());

    for (int i(0); i < _image->width(); ++i){
        for (int j(0); j < _image->height(); ++j) {
            if (qGray(_image->pixel(i,j)) < _minPixel)
                _minPixel = qGray(_image->pixel(i,j));
            if (qGray(_image->pixel(i,j)) > _maxPixel)
                _maxPixel = qGray(_image->pixel(i,j));
        }
    }

    _scalemethods = {
        {ConvolutionMatrix::Proportional, [this](int x){
            return ((x-_minPixel)/(_maxPixel-_minPixel)) * (Histogram::maxLevels-1);
        }},
        {ConvolutionMatrix::Triangle, [](int x){
            if (x < 0) return 0;
            if (x > 0) return Histogram::maxLevels - 1;
            return Histogram::maxLevels-1 / 2;
        }},
        {ConvolutionMatrix::Cut, [](int x){
            if (x < 0) return 0;
            if (x > Histogram::maxLevels-1) return Histogram::maxLevels - 1;
            return x;
        }},
    };
}

ConvolutionMatrix::~ConvolutionMatrix()
{
    delete ui;
}

QImage *ConvolutionMatrix::applyMask(const QImage *img, std::vector<std::vector<int> > mask,
                                     borderFunction bound,
                                     int kW, int kH, int divisor,
                                     std::function<int(int)> scale)
{
    QImage* res = new QImage();
    *res = img->convertToFormat(QImage::Format_Grayscale8);


    for(int i(0); i < res->width(); ++i) {
        for (int j(0); j < res->height(); ++j) {

            int avg = 0;
            for (int ii(-kW); ii <= kW; ++ii) {
                for (int jj(-kH); jj <= kH; ++jj) {
                    if (i+ii < 0 || i+ii >= img->width() || j+jj < 0 || j+jj>= img->height()){
                        avg = bound(img, i, j, divisor, mask);
                        goto border_processed;
                    }
                    avg += qGray((img->pixel(ii+i, jj+j)))*mask.at(ii+kW).at(jj+kH);
                }
            }

            avg /= divisor;
border_processed:
            avg = scale(avg);
            res->setPixel(i,j, qRgb(avg, avg, avg));

        }
    }

    return res;
}

void ConvolutionMatrix::on_maskSizeBox_currentTextChanged(const QString &item)
{
    auto rows = std::get<0>(_allowedMatrixSizes[item]);
    auto cols = std::get<1>(_allowedMatrixSizes[item]);
    _matrix.clear();

    for (int i(0); i < rows; ++i) {
        std::vector<std::unique_ptr<QSpinBox>> col;
        for (int j(0); j < cols; ++j) {
            auto ptr = std::make_unique<QSpinBox>();
            col.push_back(std::move(ptr));
            ui->matrixGrid->addWidget(col.back().get(), j, i);
            col.back()->setMinimum(-100);
            connect(col.back().get(), QOverload<int>::of(&QSpinBox::valueChanged),
                    this, &ConvolutionMatrix::on_spinBox_changed);
        }
        _matrix.push_back(std::move(col));
    }
}

void ConvolutionMatrix::on_applyButton_clicked()
{
    std::vector<std::vector<int> > mask;
    auto f = _borderMethods[ConvolutionMatrix::NoChange];
    if (ui->existingOnlyBox->isChecked())
        f = _borderMethods[ConvolutionMatrix::ExistingOnly];
    else if (ui->multiplyBox->isChecked())
        f = _borderMethods[ConvolutionMatrix::Multiply];
    else if (ui->noChangeBox->isChecked())
        f = _borderMethods[ConvolutionMatrix::NoChange];

    auto scale = _scalemethods[Proportional];
    if (ui->proportionalScaleCheck->isChecked())
        scale = _scalemethods[Proportional];
    else if (ui->triangleScaleCheck->isChecked())
        scale = _scalemethods[Triangle];
    else if (ui->cutScaleCheck->isChecked())
        scale = _scalemethods[Cut];

    for (auto& row : _matrix) {
        std::vector<int> vRow;
        for (auto& col : row) {
            vRow.push_back(col->value());
        }
        mask.push_back(vRow);
    }

    auto rows = std::get<0>(_allowedMatrixSizes[ui->maskSizeBox->currentText()]);
    auto cols = std::get<1>(_allowedMatrixSizes[ui->maskSizeBox->currentText()]);
    auto divisor = ui->divisorSpinBox->value();

    QImage* res = applyMask(_image, mask, f,
                            rows/2, cols/2, divisor, scale);

    emit setPreview(res);
}

void ConvolutionMatrix::on_autoDivisorCheckBox_stateChanged(int state)
{
    if (state == Qt::Checked) {
        ui->divisorSpinBox->setEnabled(false);
        int sum (0);
        for (auto& row : _matrix) {
            for (auto& col : row) {
                sum += col->value();
            }
        }
        ui->divisorSpinBox->setValue(sum > 0 ? sum : 1);
    }
    else {
        ui->divisorSpinBox->setEnabled(true);
    }
}

void ConvolutionMatrix::on_spinBox_changed(int i)
{
    if(ui->autoDivisorCheckBox->isChecked()) {
        emit ui->autoDivisorCheckBox->stateChanged(Qt::Checked);
    }
}
