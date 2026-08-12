#include "Model/Entities/GPolylineEntity.hpp"

namespace totcad {

GPolylineEntity::GPolylineEntity(const EntityID &id) : GEntity(id) {}

QRectF GPolylineEntity::bounds() const
{
    return vertices.boundingRect();
}

} // namespace totcad
