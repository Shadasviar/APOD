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
