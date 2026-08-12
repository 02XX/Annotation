#pragma once

#include <QColor>
#include <QString>

namespace totcad
{

struct GLayerEntity
{
    QString name{QStringLiteral("0")};
    int colorIndex{7};
    QColor color{Qt::white};
    QString lineTypeName{QStringLiteral("CONTINUOUS")};
    double lineWidth{0.0};
    bool visible{true};
};

} // namespace totcad
