#pragma once
#include "Tools/GTool.hpp"
namespace totcad {
class GRealtimeZoomTool final : public GTool {
    Q_OBJECT
public:
    explicit GRealtimeZoomTool(GView *view, QObject *parent = nullptr);
    bool mousePress(QMouseEvent *event) override;
    bool mouseMove(QMouseEvent *event) override;
    bool mouseRelease(QMouseEvent *event) override;
    QCursor cursor() const override { return Qt::SizeVerCursor; }
private:
    QPoint m_lastPosition;
    bool m_zooming{false};
};
} // namespace totcad
