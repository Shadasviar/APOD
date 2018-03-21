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

class ConvolutionMatrix : public IToolWidget
{
    Q_OBJECT

using borderFunction = std::function<int(const QImage*, int, int, int,
    std::vector<std::vector<int> >)>;

public:
    explicit ConvolutionMatrix(QImage *img, QWidget *parent = 0);
    ~ConvolutionMatrix();

    static QImage* applyMask (const QImage* img, std::vector<std::vector<int>> mask,
                              borderFunction bound,
                              int kW, int kH, int divisor);

private slots:
    void on_maskSizeBox_currentTextChanged(const QString &item);

    void on_applyButton_clicked();

private:
    Ui::ConvolutionMatrix *ui;
    QImage* _image;
    std::vector<std::vector<std::unique_ptr<QSpinBox>>> _matrix;
    enum  BorderMethods {NoChange, Multiply, ExistingOnly};

    static QMap<QString, std::pair<int, int>> _allowedMatrixSizes;
    static QMap<BorderMethods, borderFunction> _borderMethods;
};

#endif // CONVOLUTIONMATRIX_H
