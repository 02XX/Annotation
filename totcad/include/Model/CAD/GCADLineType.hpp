#pragma once

#include <QString>
#include <QVector>

namespace totcad {

struct GCADLineType {
    QString name{QStringLiteral("CONTINUOUS")};
    QString description;
    QVector<double> pattern;
};

} // namespace totcad
