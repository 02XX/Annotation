#pragma once
#include "Tools/GCADTool.hpp"
namespace totcad {
class GTypeAnnotationTool final : public GCADTool {
    Q_OBJECT
public:
    explicit GTypeAnnotationTool(GCADView *view, QObject *parent = nullptr);
    bool mousePress(QMouseEvent *event) override;
    bool keyPress(QKeyEvent *event) override;
signals:
    void assignmentRequested(const QStringList &entityIds);
};
} // namespace totcad
