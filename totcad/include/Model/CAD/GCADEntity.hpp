#pragma once

#include <QColor>
#include <QRectF>
#include <QString>

namespace totcad {

enum class GCADEntityType {
    Point,
    Line,
    Circle,
    Arc,
    Polyline,
    Text,
    Dimension,
    Insert,
    Hatch
};

class GCADEntity
{
public:
    explicit GCADEntity(GCADEntityType type);
    virtual ~GCADEntity() = default;

    GCADEntityType type() const noexcept { return m_type; }
    const QString &id() const noexcept { return m_id; }
    void setId(QString id) { m_id = std::move(id); }

    QString layerName{QStringLiteral("0")};
    QString lineTypeName{QStringLiteral("BYLAYER")};
    int colorIndex{256};
    QColor trueColor;
    bool paperSpace{false};

    virtual QRectF bounds() const = 0;

private:
    GCADEntityType m_type;
    QString m_id;
};

} // namespace totcad
