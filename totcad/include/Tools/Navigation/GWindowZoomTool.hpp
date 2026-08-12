#pragma once
#include "Tools/GTool.hpp"
#include <QRubberBand>
namespace totcad {
class GWindowZoomTool final : public GTool {
    Q_OBJECT
public:
    explicit GWindowZoomTool(GView *view, QObject *parent = nullptr);
    bool mousePress(QMouseEvent *event) override;
    bool mouseMove(QMouseEvent *event) override;
    bool mouseRelease(QMouseEvent *event) override;
    void deactivate() override;
private:
    QPoint m_start;
    QRubberBand *m_rubberBand{nullptr};
};
} // namespace totcad
