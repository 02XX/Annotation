#pragma once

#include "Model/CAD/GCADEntity.hpp"

#include <QPolygonF>
#include <QVector>

namespace totcad {

class GCADHatch final : public GCADEntity
{
public:
    GCADHatch() : GCADEntity(GCADEntityType::Hatch) {}
    QVector<QPolygonF> boundaries;
    QString patternName{QStringLiteral("SOLID")};
    bool solid{true};
    QRectF bounds() const override
    {
        QRectF result;
        for (const QPolygonF &boundary : boundaries)
            result = result.isNull() ? boundary.boundingRect() : result.united(boundary.boundingRect());
        return result;
    }
};

} // namespace totcad
