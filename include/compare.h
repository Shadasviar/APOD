#ifndef COMPARE_H
#define COMPARE_H

#include <QFrame>
#include "itoolwidget.h"
#include "scalableimageview.h"
#include "histogram.h"
#include <memory>
#include <QSplitter>

namespace Ui {
class Compare;
}

class Compare : public IToolWidget
{
    Q_OBJECT

public:
    explicit Compare(QImage* img1, QImage* img2, QWidget *parent = 0);
    ~Compare();

private:
    Ui::Compare *ui;
    QImage* _image1, *_image2;
    ScalableImageView _imageView;
    std::unique_ptr<Histogram> _hist;
    QSplitter* _splitter;
};

#endif // COMPARE_H
