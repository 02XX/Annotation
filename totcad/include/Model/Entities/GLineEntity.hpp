#pragma once

#include "Model/Entities/GEntity.hpp"

#include <QLineF>

namespace totcad {

class GLineEntity final : public GEntity
{
public:
    explicit GLineEntity(const EntityID &id);
    QPointF start;
    QPointF end;
    QRectF bounds() const override;
};

} // namespace totcad
