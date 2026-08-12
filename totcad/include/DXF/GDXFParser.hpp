#pragma once

#include "DXF/GDXFGroup.hpp"

namespace totcad {

class GCADDocument;

class GDXFParser
{
public:
    bool parseFile(const QString &filePath, GCADDocument &document, QString *errorMessage = nullptr) const;
    bool parse(const QVector<GDXFGroup> &groups, GCADDocument &document, QString *errorMessage = nullptr) const;
};

} // namespace totcad
