#include "Model/Entities/GArcEntity.hpp"

namespace totcad {

GArcEntity::GArcEntity(const EntityID &id) : GEntity(id) {}

QRectF GArcEntity::bounds() const
{
    return {center.x() - radius, center.y() - radius, radius * 2.0, radius * 2.0};
}

} // namespace totcad
