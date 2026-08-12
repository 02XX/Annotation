#pragma once
#include "Tools/GCADTool.hpp"
namespace totcad {
class GSelectTool final : public GCADTool {
    Q_OBJECT
public: explicit GSelectTool(GCADView *view, QObject *parent = nullptr);
};
} // namespace totcad
