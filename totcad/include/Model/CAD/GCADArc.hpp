#pragma once

#include "Model/CAD/GCADEntity.hpp"

namespace totcad {

class GCADArc final : public GCADEntity
{
public:
    GCADArc() : GCADEntity(GCADEntityType::Arc) {}
    QPointF center;
    double radius{0.0};
    double startAngle{0.0};
    double endAngle{0.0};
    QRectF bounds() const override { return QRectF(center.x() - radius, center.y() - radius, radius * 2.0, radius * 2.0); }
};

} // namespace totcad
