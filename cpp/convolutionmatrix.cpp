/*
 * This file is part of HarbuzHIST18. 
 * convolutionmatrix.cpp
 * Copyright (C) Uladzislau Harbuz 2018
 * 
 * HarbuzHIST18 is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * HarbuzHIST18 is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "convolutionmatrix.h"
#include "ui_convolutionmatrix.h"
#include "histogram.h"
#include <set>

QMap<QString, std::pair<int, int>> ConvolutionMatrix::_allowedMatrixSizes = {
    {"3x3", {3,3}},
    {"3x5", {3,5}},
    {"5x3", {5,3}},
    {"5x5", {5,5}},
    {"7x7", {7,7}},
};

QMap<QString, std::vector<std::vector<int>>> ConvolutionMatrix::_maskPresets = {
    {"Laplassian",      {{0,-1,0},  {-1,4,-1},  {0,-1,0}}},
    {"Laplassian 2",    {{-1,-1,-1},{-1,8,-1},  {-1,-1,-1}}},
    {"Laplassian 3",    {{0,-2,0},  {-2,4,-2},  {0,-2,0}}},
    {"Sobel 1",         {{-1,0,1},  {-2,0,2},   {-1,0,1}}},
    {"Sobel 2",         {{-1,-2,-1},{0,0,0},    {1,2,1}}},
    {"Prewitt N",       {{1,1,1},   {1,-2,1},   {-1,-1,-1}}},
    {"Prewitt NE",      {{1,1,1},   {-1,-2,1},  {-1,-1,1}}},
    {"Prewitt E",       {{-1,1,1},  {-1,-2,1},  {-1,1,1}}},
    {"Prewitt SE",      {{-1,-1,1}, {-1,-2,1},  {1,1,1}}},
    {"Prewitt S",       {{-1,-1,-1},{1,-2,1},   {1,1,1}}},
    {"Prewitt SW",      {{1,-1,-1}, {1,-2,-1},  {1,1,1}}},
    {"Prewitt W",       {{1,1,-1},  {1,-2,-1},  {1,1,-1}}},
    {"Prewitt NW",      {{1,1,1},   {1,-2,-1},  {1,-1,-1}}},
    {"Logic horizontal",{{0,0,0},   {1,0,1},    {0,0,0}}},
    {"Logic vertical",  {{0,1,0},   {0,0,0},    {0,1,0}}},
};

QMap<ConvolutionMatrix::BorderMethods, ConvolutionMatrix::borderFunction>
ConvolutionMatrix::_borderMethods = {

    {ConvolutionMatrix::NoChange, [](const QImage*& img, cr_int i, cr_int j, cr_int,
        std::vector<std::vector<int> >,
        std::function<int(int, cr_int, cr_int, cr_int, cr_int, cr_int, std::vector<std::vector<int> >&)>) {
            return qGray(img->pixel(i,j));
     }},

    {ConvolutionMatrix::Multiply, [](const QImage*& img, cr_int i, cr_int j, cr_int divisor,
        std::vector<std::vector<int> > mask,
        std::function<int(int, cr_int, cr_int, cr_int, cr_int, cr_int, std::vector<std::vector<int> >&)> f) {
            int acc(0);
            for (int ii(i-mask.size()/2); ii <= (int)(i+mask.size()/2); ++ii) {
                for (int jj(j-mask[0].size()/2); jj <= (int)(j+mask[0].size()/2); ++jj) {
                    if (ii < 0 || ii >= img->width()) {
                        if (jj >= 0 && jj < img->height())
                            acc = f(acc, qGray(img->pixel(i, jj)),i, j, ii, jj, mask);
                    }
                    else if (jj < 0 || jj >= img->height()) {
                        acc = f(acc, qGray(img->pixel(ii, j)),i, j, ii, jj, mask);
                    }
                    else
                        acc = f(acc, qGray(img->pixel(ii, jj)),i, j, ii, jj, mask);
                }
            }
            return acc / divisor;
     }},

    {ConvolutionMatrix::ExistingOnly, [](const QImage*& img, cr_int i, cr_int j, cr_int c_divisor,
        std::vector<std::vector<int> > mask,
        std::function<int(int, cr_int, cr_int, cr_int, cr_int, cr_int, std::vector<std::vector<int> >&)> f) {
         int acc(0);
         int divisor(c_divisor);
         for (int ii(i-mask.size()/2); ii <= (int)(i+mask.size()/2); ++ii) {
             for (int jj(j-mask[0].size()/2); jj <= (int)(j+mask[0].size()/2); ++jj) {
                 if (ii < 0 || ii >= img->width() || jj < 0 || jj >= img->height()) {
                     --divisor;
                     if (divisor <= 0) divisor = 1;
                 }
                 else {
                     acc = f(acc, qGray(img->pixel(ii, jj)),i, j, ii, jj, mask);
                 }
             }
         }
         return acc / divisor;
     }},
};

ConvolutionMatrix::ConvolutionMatrix(QImage* img, QWidget *parent) :
    IToolWidget(parent),
    ui(new Ui::ConvolutionMatrix),
    _image(img)
{
    ui->setupUi(this);
    emit on_maskSizeBox_currentTextChanged(ui->maskSizeBox->currentText());
    _minPixel = Settings::maxLevels;

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
            return ((x-_minPixel)/(_maxPixel-_minPixel)) * (Settings::maxLevels-1);
        }},
        {ConvolutionMatrix::Triangle, [](int x){
            if (x < 0) return 0;
            if (x > 0) return Settings::maxLevels - 1;
            return Settings::maxLevels-1 / 2;
        }},
        {ConvolutionMatrix::Cut, [](int x){
            if (x < 0) return 0;
            if (x > Settings::maxLevels-1)
                return Settings::maxLevels - 1;
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
        emit setProgressBar((100./res->width())*i);
        for (int j(0); j < res->height(); ++j) {

            int avg = 0;

            if (i-kW < 0 || i+kW >= img->width() || j-kH < 0 || j+kH>= img->height()) {
                avg = bound(img, i, j, divisor, mask,
                            [](int acc, cr_int px, cr_int i, cr_int j, cr_int ii, cr_int jj,
                            std::vector<std::vector<int> >& mask) {
                    return acc + px * mask[(ii-i) + mask.size()/2][(jj-j) + mask[0].size()/2];
                });
            }
            else {
                for (int ii(-kW); ii <= kW; ++ii) {
                    for (int jj(-kH); jj <= kH; ++jj) {
                        avg += qGray((img->pixel(ii+i, jj+j)))*mask.at(ii+kW).at(jj+kH);
                    }
                }
                avg /= divisor;
            }

            avg = scale(avg);
            res->setPixel(i,j, qRgb(avg, avg, avg));

        }
    }

    emit hideProgressBar();
    return res;
}

QImage *ConvolutionMatrix::medianFilter(const QImage *img, std::vector<std::vector<int> > mask,
                                        ConvolutionMatrix::borderFunction bound,
                                        int kW, int kH,
                                        std::function<int (int)> scale)
{
    QImage* res = new QImage();
    *res = img->convertToFormat(QImage::Format_Grayscale8);

    for(int i(0); i < res->width(); ++i) {
        emit setProgressBar((100./res->width())*i);
        for (int j(0); j < res->height(); ++j) {

            int med = 0;

            if (i-kW < 0 || i+kW >= img->width() || j-kH < 0 || j+kH>= img->height()) {
                std::multiset<int> median;

                med = bound(img, i, j, 1, mask,
                            [&median](int, cr_int px, cr_int, cr_int, cr_int, cr_int,
                            std::vector<std::vector<int> >&) {
                    median.insert(px);
                    auto it = median.begin();
                    std::advance(it, median.size() / 2);
                    return *it;
                });
                median.clear();
            }
            else {
                std::vector<int> median(mask.size() * mask[0].size());

                int idx(0);
                for (int ii(-kW); ii <= kW; ++ii) {
                    for (int jj(-kH); jj <= kH; ++jj) {
                        median[idx++] = qGray((img->pixel(ii+i, jj+j)));
                    }
                }
                std::nth_element(median.begin(), median.begin() + median.size()/2, median.end());
                med = median[median.size()/2];
            }

            med = scale(med);
            res->setPixel(i,j, qRgb(med, med, med));

        }
    }

    emit hideProgressBar();
    return res;
}

QImage *ConvolutionMatrix::logicFilter(const QImage *img, std::vector<std::vector<int> > mask,
                                       ConvolutionMatrix::borderFunction bound,
                                       int kW, int kH,
                                       std::function<int (int)> scale)
{
    QImage* res = new QImage();
    *res = img->convertToFormat(QImage::Format_Grayscale8);

    for(int i(0); i < res->width(); ++i) {
        emit setProgressBar((100./res->width())*i);
        for (int j(0); j < res->height(); ++j) {

            int px(qGray((img->pixel(i, j))));
            std::vector<int> rpx;

            if (i-kW < 0 || i+kW >= img->width() || j-kH < 0 || j+kH>= img->height()) {
                px = bound(img, i, j, 1, mask,
                           [&img](int res, cr_int px, cr_int i, cr_int j, cr_int ii, cr_int jj,
                           std::vector<std::vector<int> >& mask) {
                    if (mask[(ii-i) + mask.size()/2][(jj-j) + mask[0].size()/2])
                        res = px == res ? px : qGray(img->pixel(i,j));
                    return res;
                });
            }
            else {
                for (int ii(-kW); ii <= kW; ++ii) {
                    for (int jj(-kH); jj <= kH; ++jj) {
                        if (mask[ii+kW][jj+kH]) rpx.push_back(qGray((img->pixel(ii+i, jj+j))));
                    }
                }

                bool eq(true);
                for (int& x : rpx) eq &= x == rpx[0];

                px = eq ? rpx[0] : px;
            }

            px = scale(px);
            res->setPixel(i,j, qRgb(px, px, px));

        }
    }

    emit hideProgressBar();
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
            col.back()->setEnabled(_matrixEnabled);
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

    QImage* res;

    if (ui->filterTypeSpinBox->currentText() == "Median") {
        res = medianFilter(_image, mask, f,
                                rows/2, cols/2, scale);
    } else if (ui->filterTypeSpinBox->currentText().contains("Logic")) {
        res = logicFilter(_image, mask, f,
                                rows/2, cols/2, scale);
    } else {
        res = applyMask(_image, mask, f,
                                rows/2, cols/2, divisor, scale);
    }

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

void ConvolutionMatrix::on_spinBox_changed(int)
{
    if(ui->autoDivisorCheckBox->isChecked()) {
        emit ui->autoDivisorCheckBox->stateChanged(Qt::Checked);
    }
}

void ConvolutionMatrix::on_filterTypeSpinBox_currentIndexChanged(const QString &index)
{
    ui->maskSizeBox->setEnabled(true);
    if (index == "Median") {
        _matrixEnabled = false;
        ui->divisorSpinBox->setEnabled(false);
        ui->autoDivisorCheckBox->setEnabled(false);
    } else if (index == "Convolution mask") {
        _matrixEnabled = true;
        ui->divisorSpinBox->setEnabled(true);
        ui->autoDivisorCheckBox->setEnabled(true);
    } else {
        _matrixEnabled = true;
        ui->divisorSpinBox->setEnabled(true);
        ui->autoDivisorCheckBox->setEnabled(true);
        ui->maskSizeBox->setEnabled(false);
        setMaskFromMatrix(_maskPresets[index]);
        return;
    }
    emit on_maskSizeBox_currentTextChanged(ui->maskSizeBox->currentText());
}

void ConvolutionMatrix::setMaskFromMatrix(const std::vector<std::vector<int> > mask)
{
    ui->maskSizeBox->setCurrentText(QString("%1x%2").arg(mask.size()).arg(mask[0].size()));
    emit on_maskSizeBox_currentTextChanged(ui->maskSizeBox->currentText());

    for (uint8_t i(0); i < mask.size(); ++i) {
        for (uint8_t j(0); j < mask[i].size(); ++j) {
            _matrix[i][j]->setValue(mask[j][i]);
        }
    }
}
