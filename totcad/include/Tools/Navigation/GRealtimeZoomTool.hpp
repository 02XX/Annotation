#pragma once
#include "Tools/GCADTool.hpp"
namespace totcad {
class GRealtimeZoomTool final : public GCADTool {
    Q_OBJECT
public:
    explicit GRealtimeZoomTool(GCADView *view, QObject *parent = nullptr);
    bool mousePress(QMouseEvent *event) override;
    bool mouseMove(QMouseEvent *event) override;
    bool mouseRelease(QMouseEvent *event) override;
    QCursor cursor() const override { return Qt::SizeVerCursor; }
private:
    QPoint m_lastPosition;
    bool m_zooming{false};
};
} // namespace totcad
