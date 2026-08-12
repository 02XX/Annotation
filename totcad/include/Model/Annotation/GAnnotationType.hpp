#pragma once

#include <QColor>
#include <QString>

namespace totcad {

struct GAnnotationType {
    QString id;
    QString name;
    QColor color{Qt::red};
};

} // namespace totcad
