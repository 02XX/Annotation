#pragma once

#include "Model/Entities/GEntity.hpp"

#include <QPolygonF>
#include <QVector>

namespace totcad {

class GHatchEntity final : public GEntity
{
public:
    explicit GHatchEntity(const EntityID &id);
    QVector<QPolygonF> boundaries;
    QString patternName{QStringLiteral("SOLID")};
    bool solid{true};
    QRectF bounds() const override;
};

} // namespace totcad
