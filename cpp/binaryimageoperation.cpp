#include "binaryimageoperation.h"
#include "ui_binaryimageoperation.h"
#include <QFileDialog>
#include <cmath>

QMap<QString, std::function<uint8_t(const uint8_t, const uint8_t)>> BinaryImageOperation::_operations = {
    {"Add", [](uint8_t a, uint8_t b) {return (a + b)/2;}},
    {"Sub", [](uint8_t a, uint8_t b) {return a - b;}},
    {"Difference", [](uint8_t a, uint8_t b) {return abs(a - b);}},
    {"AND", [](uint8_t a, uint8_t b) {return a & b;}},
    {"OR", [](uint8_t a, uint8_t b) {return a | b;}},
    {"XOR", [](uint8_t a, uint8_t b) {return a ^ b;}},
};

BinaryImageOperation::BinaryImageOperation(QImage* img, QWidget *parent) :
    IToolWidget(parent),
    ui(new Ui::BinaryImageOperation),
    _image(img)
{
    ui->setupUi(this);
}

BinaryImageOperation::~BinaryImageOperation()
{
    delete ui;
}

QImage *BinaryImageOperation::reduceImages(
        const std::list<std::unique_ptr<QImage> > &images,
        std::function<uint8_t(const uint8_t, const uint8_t)> op,
        QImage* startImg)
{
    QImage* accumulator = new QImage();
    *accumulator = startImg->convertToFormat(QImage::Format_Grayscale8);
    QImage buf;
    for (auto& image : images) {
        buf = image->scaled(accumulator->size())
                .convertToFormat(QImage::Format_Grayscale8);
        uint8_t px(0);
        for (int i(0); i < accumulator->width(); ++i) {
            for (int j(0); j < accumulator->height(); ++j) {
                px = op(qGray(accumulator->pixel(i,j)),
                        qGray(buf.pixel(i,j)));
                accumulator->setPixel(i,j, qRgb(px,px,px));
            }
        }
    }
    return accumulator;
}

void BinaryImageOperation::addImage()
{
    QString filename = QFileDialog::getOpenFileName(
                this,
                "Open file",
                _lastOpenedDir.absolutePath()
                );
    _lastOpenedDir = QFileInfo(filename);
    QImage* img = new QImage(filename);
    auto* scene = new QGraphicsScene();
    scene->addPixmap(QPixmap::fromImage(*img));
    scene->setSceneRect(img->rect());
    _images.push_back(std::make_unique<QImage>());
    _images.back().reset(img);

    _views.push_back(new QGraphicsView(scene, this));
    ui->imagesFrameLayout->addWidget(_views.back());
}


void BinaryImageOperation::on_addImageButton_clicked()
{
    addImage();
}

void BinaryImageOperation::on_applyButton_clicked()
{
    QString op = ui->comboBox->currentText();
    if (_operations.count(op) > 0) {
        emit setPreview(reduceImages(_images, _operations[op], _image));
    }
}
