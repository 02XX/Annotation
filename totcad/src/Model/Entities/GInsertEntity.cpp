#include "Model/Entities/GInsertEntity.hpp"

namespace totcad {

GInsertEntity::GInsertEntity(const EntityID &id) : GEntity(id) {}

QRectF GInsertEntity::bounds() const
{
    return {position, QSizeF()};
}

} // namespace totcad
