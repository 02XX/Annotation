#pragma once

#include "Model/CAD/GCADEntity.hpp"

#include <QPointF>

namespace totcad {

class GCADPoint final : public GCADEntity
{
public:
    GCADPoint() : GCADEntity(GCADEntityType::Point) {}
    QPointF position;
    QRectF bounds() const override { return QRectF(position, QSizeF()).adjusted(-0.5, -0.5, 0.5, 0.5); }
};

} // namespace totcad
