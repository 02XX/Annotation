#pragma once

#include "Model/Entities/GEntity.hpp"

#include <QPolygonF>
#include <QVector>

namespace totcad {

class GPolylineEntity final : public GEntity
{
public:
    explicit GPolylineEntity(const EntityID &id);
    QPolygonF vertices;
    QVector<double> bulges;
    bool closed{false};
    QRectF bounds() const override;
};

} // namespace totcad
