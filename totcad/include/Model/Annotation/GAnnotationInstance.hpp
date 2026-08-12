#pragma once

#include <QSet>
#include <QString>

namespace totcad {

struct GAnnotationInstance {
    QString id;
    QString typeId;
    QString name;
    QSet<QString> entityIds;
};

} // namespace totcad
