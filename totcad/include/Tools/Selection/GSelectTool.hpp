#pragma once
#include "Tools/GTool.hpp"
namespace totcad {
class GSelectTool final : public GTool {
    Q_OBJECT
public: explicit GSelectTool(GView *view, QObject *parent = nullptr);
};
} // namespace totcad
