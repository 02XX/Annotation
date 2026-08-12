#pragma once
#include "Model/Entities/GEntity.hpp"
#include <QVector>
namespace totcad {
class GScene;
class GEntityPicker {
public:
    static QVector<EntityID> selectedEntities(const GScene &scene);
};
} // namespace totcad
