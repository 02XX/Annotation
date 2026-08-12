#pragma once
#include "Model/Entities/GEntity.hpp"
#include <QVector>
namespace totcad {
class GGraphicsScene;
class GEntityPicker final {
public:
    static QVector<EntityID> selectedEntities(const GGraphicsScene &scene);
};
} // namespace totcad
