#pragma once

#include "Model/CAD/GCADEntity.hpp"

#include <QLineF>

namespace totcad {

class GCADLine final : public GCADEntity
{
public:
    GCADLine() : GCADEntity(GCADEntityType::Line) {}
    QPointF start;
    QPointF end;
    QRectF bounds() const override { return QRectF(start, end).normalized(); }
};

} // namespace totcad
