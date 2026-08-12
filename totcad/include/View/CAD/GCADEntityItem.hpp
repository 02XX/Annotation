#pragma once

#include <QGraphicsPathItem>
#include <QBrush>
#include <QPen>

namespace totcad {

class GCADEntityItem final : public QGraphicsPathItem
{
public:
    GCADEntityItem(QString entityId, QString layerName, const QPainterPath &path);

    const QString &entityId() const noexcept { return m_entityId; }
    const QString &layerName() const noexcept { return m_layerName; }
    void setBaseStyle(const QPen &pen, const QBrush &brush);

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

private:
    void updateSelectionStyle(bool selected);
    QString m_entityId;
    QString m_layerName;
    QPen m_basePen;
    QBrush m_baseBrush;
};

} // namespace totcad
