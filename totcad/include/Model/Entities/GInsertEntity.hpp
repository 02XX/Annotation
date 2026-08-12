#pragma once

#include "Model/Entities/GEntity.hpp"

namespace totcad {

class GInsertEntity final : public GEntity
{
public:
    explicit GInsertEntity(const EntityID &id);
    QString blockName;
    QPointF position;
    double scaleX{1.0};
    double scaleY{1.0};
    double rotation{0.0};
    QRectF bounds() const override;
};

} // namespace totcad
