#pragma once

#include "Model/Entities/GEntity.hpp"

namespace totcad {

class GArcEntity final : public GEntity
{
public:
    explicit GArcEntity(const EntityID &id);
    QPointF center;
    double radius{0.0};
    double startAngle{0.0};
    double endAngle{0.0};
    QRectF bounds() const override;
};

} // namespace totcad
