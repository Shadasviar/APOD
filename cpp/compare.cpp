/*
 * This file is part of HarbuzHIST18.
 * compare.cpp
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
    Histogram* hist = new Histogram(diffImg, this, "Original and processed difference");
    ui->horizontalLayout->addWidget(_splitter);
    _hist.reset(hist);
    _imageView = ScalableImageView(diffImg);
    ui->histLayout->addWidget(_hist.get());
    ui->imageLayout->addWidget(&_imageView);
    _splitter->addWidget(ui->imageFrame);
    _splitter->addWidget(ui->histFrame);
    connect(_hist.get(), &Histogram::showStatusMsg, this, &Compare::showStatusMsg);
    connect(&_imageView, &ScalableImageView::imageChanged, _hist.get(), &IToolWidget::sourceChanged);
}

Compare::~Compare()
{
    delete ui;
}

void Compare::sourceChanged(QImage *img)
{
    if (_image2 == img) {
        _image1 = img;
    } else {
        _image2 = img;
    }
    update();
}

void Compare::update()
{
    BinaryImageOperation diffop(_image1, this);

    std::list<std::unique_ptr<QImage>> images;
    images.push_back(std::make_unique<QImage>(_image2->copy()));
    QImage* diffImg = diffop.reduceImages(images,
                                      BinaryImageOperation::operations["Difference"], _image1);
    _imageView = ScalableImageView(diffImg);
}
