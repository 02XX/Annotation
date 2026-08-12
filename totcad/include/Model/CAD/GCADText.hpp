#pragma once

#include "Model/CAD/GCADEntity.hpp"

namespace totcad {

class GCADText final : public GCADEntity
{
public:
    GCADText() : GCADEntity(GCADEntityType::Text) {}
    QPointF position;
    QString text;
    double height{2.5};
    double rotation{0.0};
    QRectF bounds() const override
    {
        const double width = qMax(height, height * 0.65 * static_cast<double>(text.size()));
        return QRectF(position.x(), position.y(), width, height);
    }
};

} // namespace totcad
