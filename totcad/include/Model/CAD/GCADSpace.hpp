#pragma once

#include <QString>
#include <QStringList>

namespace totcad {

struct GCADSpace {
    QString name;
    bool paperSpace{false};
    QStringList entityIds;
};

} // namespace totcad
