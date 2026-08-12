#pragma once

#include <QString>

namespace totcad {

class GAnnotationModel;

class GAnnotationSerializer
{
public:
    bool save(const QString &filePath, const GAnnotationModel &document, QString *errorMessage = nullptr) const;
    bool load(const QString &filePath, GAnnotationModel &document, QString *errorMessage = nullptr) const;
};

} // namespace totcad
