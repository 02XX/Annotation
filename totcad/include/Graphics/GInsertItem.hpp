#pragma once

#include "Graphics/GEntityItem.hpp"

namespace totcad {

class GInsertEntity;

class GInsertItem final : public QGraphicsItem, public GEntityItem
{
public:
    explicit GInsertItem(GInsertEntity *entity);
    GInsertEntity *entity() const noexcept { return m_entity; }
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

private:
    QPainterPath entityPath() const;
    GInsertEntity *m_entity{nullptr};
};

} // namespace totcad
