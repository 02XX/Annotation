#pragma once

#include <QGraphicsView>

namespace totcad {

class GCADTool;

class GCADView final : public QGraphicsView
{
    Q_OBJECT
public:
    explicit GCADView(QGraphicsScene *scene, QWidget *parent = nullptr);

    void setActiveTool(GCADTool *tool);
    GCADTool *activeTool() const noexcept { return m_activeTool; }
    void showAll();
    void zoomToSceneRect(const QRectF &rectangle, qreal viewportRatio = 1.0);
    void zoomBy(qreal factor, const QPoint &anchor = {});
    void panBy(const QPoint &delta);

signals:
    void mouseScenePositionChanged(const QPointF &position);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private:
    GCADTool *m_activeTool{nullptr};
    QPoint m_selectionStart;
    QPoint m_middlePanPosition;
    bool m_middlePanning{false};
};

} // namespace totcad
