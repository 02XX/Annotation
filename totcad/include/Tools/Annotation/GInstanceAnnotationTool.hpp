#pragma once
#include "Tools/GCADTool.hpp"
namespace totcad {
class GInstanceAnnotationTool final : public GCADTool {
    Q_OBJECT
public:
    explicit GInstanceAnnotationTool(GCADView *view, QObject *parent = nullptr);
    bool mousePress(QMouseEvent *event) override;
    bool keyPress(QKeyEvent *event) override;
signals:
    void assignmentRequested(const QStringList &entityIds);
};
} // namespace totcad
