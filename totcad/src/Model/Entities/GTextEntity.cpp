#include "Model/Entities/GTextEntity.hpp"

namespace totcad {

GTextEntity::GTextEntity(const EntityID &id) : GEntity(id) {}

QRectF GTextEntity::bounds() const
{
    const double width = qMax(height, height * 0.65 * static_cast<double>(text.size()));
    return {position.x(), position.y(), width, height};
}

} // namespace totcad
