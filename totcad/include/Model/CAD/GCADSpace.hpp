#pragma once

#include <QString>
#include <QStringList>

namespace totcad {

struct GCADSpace {
    GCADSpace() = default;
    GCADSpace(const QString &spaceName, bool isPaperSpace, const QStringList &ids = QStringList())
        : name(spaceName), paperSpace(isPaperSpace), entityIds(ids)
    {
    }

    QString name;
    bool paperSpace{false};
    QStringList entityIds;
};

} // namespace totcad
