#pragma once
#include "Tools/GCADTool.hpp"
namespace totcad {
class GPanTool final : public GCADTool {
    Q_OBJECT
public:
    explicit GPanTool(GCADView *view, QObject *parent = nullptr);
    bool mousePress(QMouseEvent *event) override;
    bool mouseMove(QMouseEvent *event) override;
    bool mouseRelease(QMouseEvent *event) override;
    QCursor cursor() const override { return Qt::OpenHandCursor; }
private:
    QPoint m_lastPosition;
    bool m_panning{false};
};
} // namespace totcad
