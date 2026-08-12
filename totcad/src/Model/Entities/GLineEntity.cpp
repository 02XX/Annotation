#include "Model/Entities/GLineEntity.hpp"

namespace totcad {

GLineEntity::GLineEntity(const EntityID &id) : GEntity(id) {}

QRectF GLineEntity::bounds() const
{
    return QRectF(start, end).normalized();
}

} // namespace totcad
