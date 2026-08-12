#include "Model/Entities/GPointEntity.hpp"

namespace totcad {

GPointEntity::GPointEntity(const EntityID &id) : GEntity(id) {}

QRectF GPointEntity::bounds() const
{
    return QRectF(position, QSizeF()).adjusted(-0.5, -0.5, 0.5, 0.5);
}

} // namespace totcad
