#pragma once

#include "DXF/GDXFGroup.hpp"

namespace totcad {

class GDocumentEntity;

class GDXFParser
{
public:
    bool parseFile(const QString &filePath, GDocumentEntity &document, QString *errorMessage = nullptr) const;
    bool parse(const QVector<GDXFGroup> &groups, GDocumentEntity &document, QString *errorMessage = nullptr) const;
};

} // namespace totcad
