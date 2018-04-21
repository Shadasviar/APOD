#include "compare.h"
#include "ui_compare.h"
#include "binaryimageoperation.h"
#include <list>

Compare::Compare(QImage *img1, QImage *img2, QWidget *parent) :
    IToolWidget(parent),
    ui(new Ui::Compare),
    _image1(img1),
    _image2(img2),
    _hist(nullptr),
    _splitter(new QSplitter(this))
{
    ui->setupUi(this);
    BinaryImageOperation diffop(_image1, this);

    std::list<std::unique_ptr<QImage>> images;
    images.push_back(std::make_unique<QImage>(_image2->copy()));

    QImage* diffImg = diffop.reduceImages(images,
                                      BinaryImageOperation::operations["Difference"], _image1);
    Histogram* hist = new Histogram(diffImg);
    ui->horizontalLayout->addWidget(_splitter);
    _hist.reset(hist);
    _imageView = ScalableImageView(diffImg);
    ui->histLayout->addWidget(_hist.get());
    ui->imageLayout->addWidget(&_imageView);
    _splitter->addWidget(ui->imageFrame);
    _splitter->addWidget(ui->histFrame);
    connect(_hist.get(), &Histogram::showStatusMsg, this, &Compare::showStatusMsg);
}

Compare::~Compare()
{
    delete ui;
}
