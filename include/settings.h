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

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QFrame>
#include "itoolwidget.h"

namespace Ui {
class Settings;
}

class Settings : public IToolWidget
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = 0);
    Settings(QImage*, QWidget *parent= 0):Settings(parent){}

    static int grayCurrLvl(QRgb rgb);
    static int to256gray(int x);

    static int maxLevels;
    ~Settings();

private slots:
    void on_horizontalSlider_valueChanged(int value);

private:
    Ui::Settings *ui;
};

#endif // SETTINGS_H
