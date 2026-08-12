#pragma once

#include <QColor>
#include <QString>

namespace totcad {

struct GAnnotationType {
    GAnnotationType() = default;
    GAnnotationType(const QString &typeId, const QString &typeName, const QColor &typeColor)
        : id(typeId), name(typeName), color(typeColor)
    {
    }

    QString id;
    QString name;
    QColor color{Qt::red};
};

} // namespace totcad
