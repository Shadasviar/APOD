/*
 * This file is part of HarbuzHIST18. 
 * toolsarea.cpp
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
#include "toolsarea.h"

ToolsArea::ToolsArea(QWidget *parent) : QWidget(parent),
    _layout(this)
{

}

void ToolsArea::setTool(QWidget *tool)
{
    _activeTool.reset(tool);
    _layout.addWidget(tool);
}

ToolsArea::~ToolsArea()
{
    for (auto& key : _enabledInfoTools.keys()) {
        delete _enabledInfoTools[key];
    }
    _enabledInfoTools.clear();
}
