/*
 * This file is part of HarbuzHIST18.
 * imageworkspaceview.cpp
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

#include "imageworkspaceview.h"
#include "ui_imageworkspaceview.h"

ImageWorkspaceView::ImageWorkspaceView(QWidget *parent) :
    IToolWidget(parent),
    ui(new Ui::ImageWorkspaceView),
    _tools(this),
    _splitter(new QSplitter(this)),
    _imageSplitter(new QSplitter(Qt::Vertical ,this))
{
    ui->setupUi(this);
    ui->verticalLayout->addWidget(_imageSplitter);
    _imageSplitter->addWidget(&_image);
    ui->horizontalLayout->addWidget(_splitter);
    _splitter->addWidget(ui->imagesFrame);
    _splitter->addWidget(ui->scrollArea);
    ui->scrollArea->setWidget(&_tools);
}

ImageWorkspaceView::ImageWorkspaceView(QImage &&image, QWidget *parent):
    ImageWorkspaceView(parent)
{
    auto* img = new QImage();
    *img = QImage(image).convertToFormat(QImage::Format_Grayscale8);
    for (int i(0); i < img->width(); ++i)
        for (int j(0); j < img->height(); ++j) {
            int px = Settings::to256gray(
                        Settings::grayCurrLvl(img->pixel(i, j))
                        );
            img->setPixel(i, j, qRgb(px, px, px));
        }
    _image = ScalableImageView(img, this);
}

ImageWorkspaceView::~ImageWorkspaceView()
{
    delete ui;
}

void ImageWorkspaceView::deleteActiveTool()
{
    _tools.deleteTool();
}

void ImageWorkspaceView::modifyPreview(QImage *img)
{
    if(!_preview.getImage()) {
        _imageSplitter->addWidget(&_preview);
    }
    _preview = ScalableImageView(img, this);
}
