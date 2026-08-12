#pragma once

#include <QString>

namespace totcad {

struct GDXFGroup {
    GDXFGroup() = default;
    GDXFGroup(int groupCode, const QString &groupValue, int sourceLine)
        : code(groupCode), value(groupValue), lineNumber(sourceLine)
    {
    }

    int code{0};
    QString value;
    int lineNumber{0};

    int toInt(int fallback = 0) const
    {
        bool ok = false;
        const int result = value.trimmed().toInt(&ok);
        return ok ? result : fallback;
    }

    double toDouble(double fallback = 0.0) const
    {
        bool ok = false;
        const double result = value.trimmed().toDouble(&ok);
        return ok ? result : fallback;
    }
};

} // namespace totcad
