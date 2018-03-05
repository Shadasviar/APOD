#include "toolsarea.h"

ToolsArea::ToolsArea(QWidget *parent) : QWidget(parent),
    _layout(this)
{

}

ToolsArea::~ToolsArea()
{
    for (auto& key : _enabledTools.keys()) {
        delete _enabledTools[key];
    }
    _enabledTools.clear();
}
