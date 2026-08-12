#pragma once

#include "Model/Entities/GEntity.hpp"

#include <QPointF>

namespace totcad {

class GPointEntity final : public GEntity
{
public:
    explicit GPointEntity(const EntityID &id);
    QPointF position;
    QRectF bounds() const override;
};

} // namespace totcad
