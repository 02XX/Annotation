#include "Model/Entities/GEntity.hpp"

namespace totcad {

GEntity::GEntity(const EntityID &id) : Entity<EntityID>(id)
{
}

GEntity::~GEntity() = default;

QRectF GEntity::bounds() const
{
    return {};
}

} // namespace totcad
