/*
 * This file is part of HarbuzHIST18.
 * compare.h
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
#ifndef COMPARE_H
#define COMPARE_H

#include <QFrame>
#include "itoolwidget.h"
#include "scalableimageview.h"
#include "histogram.h"
#include <memory>
#include <QSplitter>

namespace Ui {
class Compare;
}

class Compare : public IToolWidget
{
    Q_OBJECT

public:
    explicit Compare(QImage* img1, QImage* img2, QWidget *parent = 0);
    ~Compare();

protected slots:
    void sourceChanged(QImage* img);

signals:
    void sigSuorceChanged(QImage*);

private:
    Ui::Compare *ui;
    QImage* _image1, *_image2;
    ScalableImageView _imageView;
    std::unique_ptr<Histogram> _hist;
    QSplitter* _splitter;
    void update();
};

#endif // COMPARE_H
