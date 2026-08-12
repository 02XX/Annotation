#pragma once

#include <QGraphicsView>

namespace annotation
{
class GGraphicsView : public QGraphicsView
{
  public:
    explicit GGraphicsView(QGraphicsScene *scene, QWidget *parent = nullptr);
};
} // namespace annotation
