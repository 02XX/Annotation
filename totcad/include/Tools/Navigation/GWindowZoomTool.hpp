#pragma once
#include "Tools/GCADTool.hpp"
#include <QRubberBand>
namespace totcad {
class GWindowZoomTool final : public GCADTool {
    Q_OBJECT
public:
    explicit GWindowZoomTool(GCADView *view, QObject *parent = nullptr);
    bool mousePress(QMouseEvent *event) override;
    bool mouseMove(QMouseEvent *event) override;
    bool mouseRelease(QMouseEvent *event) override;
    void deactivate() override;
private:
    QPoint m_start;
    QRubberBand *m_rubberBand{nullptr};
};
} // namespace totcad
