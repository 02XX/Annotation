#include "Model/Entities/GDimensionEntity.hpp"

namespace totcad {

GDimensionEntity::GDimensionEntity(const EntityID &id) : GEntity(id) {}

QRectF GDimensionEntity::bounds() const
{
    return QRectF(definitionPoint, textPoint).normalized();
}

} // namespace totcad
