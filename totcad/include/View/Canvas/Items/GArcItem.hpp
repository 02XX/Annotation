#pragma once

#include "View/Canvas/Items/GEntityItem.hpp"

namespace totcad {

class GArcEntity;

class GArcItem final : public QGraphicsItem, public GEntityItem
{
public:
    explicit GArcItem(GArcEntity *entity);
    GArcEntity *entity() const noexcept { return m_entity; }
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

private:
    QPainterPath entityPath() const;
    GArcEntity *m_entity{nullptr};
};

} // namespace totcad
