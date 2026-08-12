#pragma once

#include <QString>
#include <QVector>

namespace totcad {

struct GLineTypeEntity {
    QString name{QStringLiteral("CONTINUOUS")};
    QString description;
    QVector<double> pattern;
};

} // namespace totcad
