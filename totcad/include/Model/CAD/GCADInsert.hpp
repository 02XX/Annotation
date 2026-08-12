#pragma once

#include "Model/CAD/GCADEntity.hpp"

namespace totcad {

class GCADInsert final : public GCADEntity
{
public:
    GCADInsert() : GCADEntity(GCADEntityType::Insert) {}
    QString blockName;
    QPointF position;
    double scaleX{1.0};
    double scaleY{1.0};
    double rotation{0.0};
    QRectF bounds() const override { return QRectF(position, QSizeF()); }
};

} // namespace totcad
