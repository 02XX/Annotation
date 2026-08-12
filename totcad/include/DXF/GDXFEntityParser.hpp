#pragma once

#include "DXF/GDXFGroup.hpp"

#include <memory>

namespace totcad {

class GCADEntity;

class GDXFEntityParser
{
public:
    std::shared_ptr<GCADEntity> parse(const QString &entityName,
                                     const QVector<GDXFGroup> &groups,
                                     const QString &fallbackId) const;
};

} // namespace totcad
