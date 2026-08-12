#pragma once

#include "Model/CAD/GCADEntity.hpp"

#include <QPointF>
#include <memory>
#include <vector>

namespace totcad {

struct GCADBlock {
    QString name;
    QPointF basePoint;
    std::vector<std::shared_ptr<GCADEntity>> entities;
};

} // namespace totcad
