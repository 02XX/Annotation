#pragma once

#include "Model/Entities/GEntity.hpp"

#include <QPointF>
#include <memory>
#include <vector>

namespace totcad {

struct GBlockEntity {
    QString name;
    QPointF basePoint;
    std::vector<std::shared_ptr<GEntity>> entities;
};

} // namespace totcad
