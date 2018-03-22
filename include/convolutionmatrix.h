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
                              int kW, int kH, int divisor, std::function<int(int)> scale);

private slots:
    void on_maskSizeBox_currentTextChanged(const QString &item);
    void on_applyButton_clicked();
    void on_autoDivisorCheckBox_stateChanged(int state);
    void on_spinBox_changed(int i);

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
