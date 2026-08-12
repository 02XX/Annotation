#include "Model/Entities/GCircleEntity.hpp"

namespace totcad {

GCircleEntity::GCircleEntity(const EntityID &id) : GEntity(id) {}

QRectF GCircleEntity::bounds() const
{
    return {center.x() - radius, center.y() - radius, radius * 2.0, radius * 2.0};
}

} // namespace totcad
