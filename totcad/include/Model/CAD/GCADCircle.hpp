#pragma once

#include "Model/CAD/GCADEntity.hpp"

namespace totcad {

class GCADCircle final : public GCADEntity
{
public:
    GCADCircle() : GCADEntity(GCADEntityType::Circle) {}
    QPointF center;
    double radius{0.0};
    QRectF bounds() const override { return QRectF(center.x() - radius, center.y() - radius, radius * 2.0, radius * 2.0); }
};

} // namespace totcad
