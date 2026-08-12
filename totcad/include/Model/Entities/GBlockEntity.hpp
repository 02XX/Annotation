#pragma once

#include "Model/Entities/GEntity.hpp"

#include <QPointF>
#include <QVector>
#include <memory>

namespace totcad {

struct GBlockEntity {
    QString name;
    QPointF basePoint;
    QVector<std::shared_ptr<GEntity>> entities;
};

} // namespace totcad
