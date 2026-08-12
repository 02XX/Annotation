#pragma once

#include "Model/Entities/GEntity.hpp"

namespace totcad {

class GDimensionEntity final : public GEntity
{
public:
    explicit GDimensionEntity(const EntityID &id);
    QPointF definitionPoint;
    QPointF textPoint;
    QString text;
    QString blockName;
    QRectF bounds() const override;
};

} // namespace totcad
