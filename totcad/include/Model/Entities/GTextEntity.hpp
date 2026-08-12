#pragma once

#include "Model/Entities/GEntity.hpp"

namespace totcad {

class GTextEntity final : public GEntity
{
public:
    explicit GTextEntity(const EntityID &id);
    QPointF position;
    QString text;
    double height{2.5};
    double rotation{0.0};
    QRectF bounds() const override;
};

} // namespace totcad
