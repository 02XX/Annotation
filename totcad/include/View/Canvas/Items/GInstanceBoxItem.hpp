#pragma once

#include <QGraphicsRectItem>

namespace totcad {

// Visual outline for an annotated entity instance.
class GInstanceBoxItem final : public QGraphicsRectItem
{
public:
    GInstanceBoxItem(QString instanceId, const QRectF &rectangle, const QColor &color);
    const QString &instanceId() const noexcept { return m_instanceId; }

private:
    QString m_instanceId;
};

} // namespace totcad
