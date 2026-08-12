#pragma once
#include "View/Canvas/Tools/GCanvasTool.hpp"
namespace totcad
{
class GSelectTool final : public GCanvasTool
{
    Q_OBJECT
  public:
    explicit GSelectTool(GGraphicsView *view, QObject *parent = nullptr);
};
} // namespace totcad
