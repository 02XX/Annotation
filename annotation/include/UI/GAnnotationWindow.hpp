#pragma once

#include <QMainWindow>

namespace annotation
{
class GGraphicsScene;
class GGraphicsView;
class GAnnotationWindow : public QMainWindow
{
  private:
    GGraphicsView *m_graphicsView{nullptr};
    GGraphicsScene *m_graphicsScene{nullptr};

  public:
    explicit GAnnotationWindow(QWidget *parent = nullptr);
};
} // namespace annotation
