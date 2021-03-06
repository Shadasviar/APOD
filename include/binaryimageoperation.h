/*
 * This file is part of HarbuzHIST18. 
 * binaryimageoperation.h
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
#ifndef BINARYIMAGEOPERATION_H
#define BINARYIMAGEOPERATION_H

#include <QFrame>
#include "itoolwidget.h"
#include <QFileInfo>
#include <QToolButton>
#include <QList>
#include <QGraphicsView>
#include <memory>
#include <list>
#include <QMap>
#include <functional>
#include "scalableimageview.h"

namespace Ui {
class BinaryImageOperation;
}

class BinaryImageOperation : public IToolWidget
{
    Q_OBJECT

public:
    explicit BinaryImageOperation(QImage *img, QWidget *parent = 0);
    ~BinaryImageOperation();
    QImage* reduceImages(std::list<ScalableImageView> &images,
                                std::function<uint8_t(const uint8_t, const uint8_t)> op,
                                QImage *startImg);

    static QMap<QString, std::function<uint8_t(const uint8_t, const uint8_t)>> operations;

private slots:

    void on_addImageButton_clicked();

    void on_applyButton_clicked();

private:
    Ui::BinaryImageOperation *ui;
    std::list<ScalableImageView> _images;
    QFileInfo _lastOpenedDir;
    QImage* _image;
    void addImage();
};

#endif // BINARYIMAGEOPERATION_H
