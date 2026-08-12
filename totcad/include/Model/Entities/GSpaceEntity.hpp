#pragma once

#include "Model/Entities/GEntity.hpp"

#include <QString>
#include <QVector>

namespace totcad {

struct GSpaceEntity {
    QString name;
    bool paperSpace{false};
    QVector<EntityID> entityIds;
};

} // namespace totcad
