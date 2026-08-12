#pragma once

#include "Model/Entities/GEntity.hpp"

#include <QSet>
#include <QString>

namespace totcad {

struct GAnnotationInstance {
    QString id;
    QString typeId;
    QString name;
    QSet<EntityID> entityIds;
};

} // namespace totcad
