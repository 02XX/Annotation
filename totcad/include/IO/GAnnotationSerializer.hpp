#pragma once

#include <QString>

namespace totcad {

class GAnnotationDocument;

class GAnnotationSerializer
{
public:
    bool save(const QString &filePath, const GAnnotationDocument &document, QString *errorMessage = nullptr) const;
    bool load(const QString &filePath, GAnnotationDocument &document, QString *errorMessage = nullptr) const;
};

} // namespace totcad
