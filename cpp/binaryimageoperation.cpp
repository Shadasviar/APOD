/*
 * This file is part of HarbuzHIST18. 
 * binaryimageoperation.cpp
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

#include "binaryimageoperation.h"
#include "ui_binaryimageoperation.h"
#include "settings.h"
#include <QFileDialog>
#include <cmath>

QMap<QString, std::function<uint8_t(const uint8_t, const uint8_t)>> BinaryImageOperation::operations = {
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

QImage *BinaryImageOperation::reduceImages(std::list<ScalableImageView> &images,
        std::function<uint8_t(const uint8_t, const uint8_t)> op,
        QImage* startImg)
{
    QImage* accumulator = new QImage();
    *accumulator = startImg->convertToFormat(QImage::Format_Grayscale8);
    QImage buf;
    for (auto& image : images) {
        buf = image.getImage()->scaled(accumulator->size())
                .convertToFormat(QImage::Format_Grayscale8);
        uint8_t px(0);
        for (int i(0); i < accumulator->width(); ++i) {
            emit setProgressBar((100./accumulator->width())*i);

            for (int j(0); j < accumulator->height(); ++j) {
                px = Settings::to256gray(op(Settings::grayCurrLvl(accumulator->pixel(i,j)),
                        Settings::grayCurrLvl(buf.pixel(i,j))));
                accumulator->setPixel(i,j, qRgb(px,px,px));
            }
        }
    }
    emit hideProgressBar();
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
    _images.push_back(ScalableImageView(img, this));

    ui->imagesFrameLayout->addWidget(&_images.back());
}


void BinaryImageOperation::on_addImageButton_clicked()
{
    addImage();
}

void BinaryImageOperation::on_applyButton_clicked()
{

    QString op = ui->comboBox->currentText();
    if (operations.count(op) > 0) {
        emit setPreview(reduceImages(_images, operations[op], _image));
    }
}
