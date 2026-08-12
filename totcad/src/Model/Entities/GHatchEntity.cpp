#include "Model/Entities/GHatchEntity.hpp"

namespace totcad {

GHatchEntity::GHatchEntity(const EntityID &id) : GEntity(id) {}

QRectF GHatchEntity::bounds() const
{
    QRectF result;
    for (const QPolygonF &boundary : boundaries)
        result = result.isNull() ? boundary.boundingRect() : result.united(boundary.boundingRect());
    return result;
}

} // namespace totcad
