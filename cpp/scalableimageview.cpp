/*
 * This file is part of HarbuzHIST18.
 * scalableimageview.cpp
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
#include "scalableimageview.h"
#include "ui_scalableimageview.h"
#include "settings.h"

ScalableImageView::ScalableImageView(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ScalableImageView)
{
    ui->setupUi(this);
}

ScalableImageView::ScalableImageView(QImage* image, QWidget *parent) :
    ScalableImageView(parent)
{
    _image = image;
    _scene.clear();
    _scene.addPixmap(QPixmap::fromImage(*_image));
    _scene.setSceneRect(_image->rect());
    ui->graphicsView->setScene(&_scene);
}

ScalableImageView &ScalableImageView::operator=(ScalableImageView &&rhs)
{
    qSwap(_image, rhs._image);
    _scene.clear();
    _scene.addPixmap(QPixmap::fromImage(*_image));
    _scene.setSceneRect(_image->rect());
    ui->graphicsView->setScene(&_scene);
    emit on_horizontalSlider_valueChanged(ui->horizontalSlider->value());
    emit imageChanged(_image);

    return *this;
}

ScalableImageView::~ScalableImageView()
{
    delete _image;
    delete ui;
}

void ScalableImageView::on_horizontalSlider_valueChanged(int value)
{
    double scale = value / 100.;
    auto img = _image->scaled(_image->width()*scale, _image->height()*scale);
    _scene.clear();
    _scene.addPixmap(QPixmap::fromImage(img));
    _scene.setSceneRect(img.rect());
    ui->graphicsView->setScene(&_scene);
    ui->labelMax->setText(QString("+ (%1%)").arg(value));
}
