#pragma once

#include "DXF/GDXFGroup.hpp"
#include "Model/Entities/GEntity.hpp"

#include <memory>

namespace totcad {

class GEntity;

class GDXFEntityParser
{
public:
    std::shared_ptr<GEntity> parse(const QString &entityName,
                                     const QVector<GDXFGroup> &groups,
                                     EntityID fallbackId) const;
};

} // namespace totcad
