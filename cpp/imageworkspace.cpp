/*
 * This file is part of HarbuzHIST18. 
 * imageworkspace.cpp
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
#include "imageworkspace.h"
#include "settings.h"

ImageWorkspace::ImageWorkspace(QWidget *parent) : QWidget(parent),
    _layout(this),
    _imagesViews(this),
    _imagesLayout(&_imagesViews),
    _splitter(new QSplitter(this)),
    _imageSplitter(new QSplitter(Qt::Vertical ,this)),
    _tools(this),
    _parent(parent)
{
    _imagesLayout.addWidget(_imageSplitter);
    _imageSplitter->addWidget(&_image);
    _layout.addWidget(_splitter);
    _splitter->addWidget(&_imagesViews);
    _splitter->addWidget(&_tools);
}

ImageWorkspace::ImageWorkspace(QImage &&image, QWidget *parent): ImageWorkspace(parent)
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

void ImageWorkspace::deleteActiveTool()
{
    _tools.deleteTool();
}

ImageWorkspace::~ImageWorkspace()
{
}

void ImageWorkspace::modifyPreview(QImage *img)
{
    if(!_preview.getImage()) {
        _imageSplitter->addWidget(&_preview);
    }
    _preview = ScalableImageView(img, this);
}
