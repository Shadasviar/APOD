#include "toolsarea.h"

ToolsArea::ToolsArea(QWidget *parent) : QWidget(parent),
    _layout(this)
{

}

void ToolsArea::addTool(QWidget *tool, ToolsArea::AllowedTools type)
{
    if (!_enabledTools.contains(type)) {
        _enabledTools[type] = tool;
        _layout.addWidget(tool);
    }
}

void ToolsArea::deleteTool(ToolsArea::AllowedTools type)
{
    delete _enabledTools[type];
    _enabledTools.remove(type);
}
