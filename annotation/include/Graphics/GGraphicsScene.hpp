#pragma once

#include <QGraphicsScene>

namespace annotation
{
class GGraphicsScene : public QGraphicsScene
{
  public:
    explicit GGraphicsScene(QWidget *parent = nullptr);
};
} // namespace annotation
