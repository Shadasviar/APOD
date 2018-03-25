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

ImageWorkspace::ImageWorkspace(QWidget *parent) : QWidget(parent),
    _layout(this),
    _imagesViews(this),
    _imagesLayout(&_imagesViews),
    _splitter(new QSplitter(this)),
    _imageSplitter(new QSplitter(Qt::Vertical ,this)),
    _imageView(std::make_unique<QGraphicsView>(_splitter)),
    _tools(this)
{
    _imagesLayout.addWidget(_imageSplitter);
    _imageSplitter->addWidget(_imageView.get());
    _layout.addWidget(_splitter);
    _splitter->addWidget(&_imagesViews);
    _splitter->addWidget(&_tools);
}

ImageWorkspace::ImageWorkspace(QImage &&image, QWidget *parent): ImageWorkspace(parent)
{
    _image = QImage(image);
    _scene.clear();
    _scene.addPixmap(QPixmap::fromImage(_image));
    _scene.setSceneRect(_image.rect());
    _imageView->setScene(&_scene);
}

void ImageWorkspace::deleteActiveTool()
{
    _tools.deleteTool();
}

ImageWorkspace::~ImageWorkspace()
{
    _scene.clear();
}

void ImageWorkspace::modifyPreview(QImage *img)
{
    if (!_preview) {
        _preview = std::make_unique<QGraphicsView>(_splitter);
        _imageSplitter->addWidget(_preview.get());
    }
    if (_preview) {
        delete _previewImage;
        _previewImage = img;
        _previewScene.clear();
        _previewScene.addPixmap(QPixmap::fromImage(*img));
        _previewScene.setSceneRect(img->rect());
        _preview->setScene(&_previewScene);
    }
}
