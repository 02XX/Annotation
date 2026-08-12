#pragma once
#include "View/Canvas/Tools/GCanvasTool.hpp"
namespace totcad {
class GRealtimeZoomTool final : public GCanvasTool {
    Q_OBJECT
public:
    explicit GRealtimeZoomTool(GGraphicsView *view, QObject *parent = nullptr);
    bool mousePress(QMouseEvent *event) override;
    bool mouseMove(QMouseEvent *event) override;
    bool mouseRelease(QMouseEvent *event) override;
    QCursor cursor() const override { return Qt::SizeVerCursor; }
private:
    QPoint m_lastPosition;
    bool m_zooming{false};
};
} // namespace totcad
