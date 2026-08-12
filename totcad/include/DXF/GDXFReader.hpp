#pragma once

#include "DXF/GDXFGroup.hpp"

#include <QVector>

namespace totcad {

class GDXFReader
{
public:
    bool readFile(const QString &filePath, QVector<GDXFGroup> &groups, QString *errorMessage = nullptr) const;
};

} // namespace totcad
