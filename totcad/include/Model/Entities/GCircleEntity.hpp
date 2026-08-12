#pragma once

#include "Model/Entities/GEntity.hpp"

namespace totcad {

class GCircleEntity final : public GEntity
{
public:
    explicit GCircleEntity(const EntityID &id);
    QPointF center;
    double radius{0.0};
    QRectF bounds() const override;
};

} // namespace totcad
