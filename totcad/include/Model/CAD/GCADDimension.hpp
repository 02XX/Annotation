#pragma once

#include "Model/CAD/GCADEntity.hpp"

namespace totcad {

class GCADDimension final : public GCADEntity
{
public:
    GCADDimension() : GCADEntity(GCADEntityType::Dimension) {}
    QPointF definitionPoint;
    QPointF textPoint;
    QString text;
    QString blockName;
    QRectF bounds() const override { return QRectF(definitionPoint, textPoint).normalized(); }
};

} // namespace totcad
