/*
 * This file is part of HarbuzHIST18. 
 * convolutionmatrix.h
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
#ifndef CONVOLUTIONMATRIX_H
#define CONVOLUTIONMATRIX_H

#include <QFrame>
#include "itoolwidget.h"
#include <vector>
#include <memory>
#include <QSpinBox>
#include <QMap>
#include <functional>
#include <array>

namespace Ui {
class ConvolutionMatrix;
}
using cr_int = const int&;

class ConvolutionMatrix : public IToolWidget
{
    Q_OBJECT

using borderFunction = std::function<int(const QImage*&, cr_int, cr_int, cr_int,
    std::vector<std::vector<int> >,
    std::function<int(int, cr_int, cr_int, cr_int, cr_int, cr_int, void*)>)>;

public:
    explicit ConvolutionMatrix(QImage *img, QWidget *parent = 0);
    ~ConvolutionMatrix();

    static QImage* applyMask (const QImage* img, std::vector<std::vector<int>> mask,
                              borderFunction bound,
                              int kW, int kH, int divisor, std::function<int(int)> scale);

    static QImage* medianFilter (const QImage* img, std::vector<std::vector<int> > mask,
                                 borderFunction bound,
                                 int kW, int kH, std::function<int(int)> scale);

private slots:
    void on_maskSizeBox_currentTextChanged(const QString &item);
    void on_applyButton_clicked();
    void on_autoDivisorCheckBox_stateChanged(int state);
    void on_spinBox_changed(int i);
    void on_filterTypeSpinBox_currentIndexChanged(const QString &index);

private:
    Ui::ConvolutionMatrix *ui;
    QImage* _image;
    std::vector<std::vector<std::unique_ptr<QSpinBox>>> _matrix;
    enum BorderMethods {NoChange, Multiply, ExistingOnly};
    enum ScaleMethods {Triangle, Cut, Proportional};

    static QMap<QString, std::pair<int, int>> _allowedMatrixSizes;
    static QMap<BorderMethods, borderFunction> _borderMethods;
    QMap<ScaleMethods, std::function<int(int)>> _scalemethods;
    int _minPixel = 256;
    int _maxPixel = 0;
};

#endif // CONVOLUTIONMATRIX_H
