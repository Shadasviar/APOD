/*
 * This file is part of HarbuzHIST18.
 * settings.cpp
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

#include "settings.h"
#include "ui_settings.h"

int Settings::maxLevels = 256;

Settings::Settings(QImage *img, QWidget *parent):
    IToolWidget(parent),
    ui(new Ui::Settings),
    _image(img)
{
    ui->setupUi(this);
    ui->horizontalSlider->setValue(maxLevels);
    ui->label_current_level->setText(QString::number(maxLevels));
}

int Settings::grayCurrLvl(QRgb rgb)
{
    return qGray(rgb)/(255./(maxLevels-1));
}

int Settings::to256gray(int x)
{
    return (x*(255/(maxLevels-1)))%256;
}


Settings::~Settings()
{
    delete ui;
}

void Settings::on_horizontalSlider_valueChanged(int value)
{
    maxLevels = value;
    ui->label_current_level->setText(QString::number(value));

    auto* img = new QImage();
    *img = _image->convertToFormat(QImage::Format_Grayscale8);
    for (int i(0); i < img->width(); ++i)
        for (int j(0); j < img->height(); ++j) {
            int px = to256gray(
                        grayCurrLvl(img->pixel(i, j))
                        );
            img->setPixel(i, j, qRgb(px, px, px));
        }

    emit setPreview(img);
}
