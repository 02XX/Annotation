#pragma once

#include "Model/CAD/GCADEntity.hpp"

#include <QPolygonF>
#include <QVector>

namespace totcad {

class GCADPolyline final : public GCADEntity
{
public:
    GCADPolyline() : GCADEntity(GCADEntityType::Polyline) {}
    QPolygonF vertices;
    QVector<double> bulges;
    bool closed{false};
    QRectF bounds() const override { return vertices.boundingRect(); }
};

} // namespace totcad
