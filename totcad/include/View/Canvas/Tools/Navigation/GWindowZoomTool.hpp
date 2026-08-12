#pragma once
#include "View/Canvas/Tools/GCanvasTool.hpp"
#include <QRubberBand>
namespace totcad {
class GWindowZoomTool final : public GCanvasTool {
    Q_OBJECT
public:
    explicit GWindowZoomTool(GGraphicsView *view, QObject *parent = nullptr);
    bool mousePress(QMouseEvent *event) override;
    bool mouseMove(QMouseEvent *event) override;
    bool mouseRelease(QMouseEvent *event) override;
    void deactivate() override;
private:
    QPoint m_start;
    QRubberBand *m_rubberBand{nullptr};
};
} // namespace totcad
