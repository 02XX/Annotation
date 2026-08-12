#pragma once
#include "Model/Entities/GEntity.hpp"
#include "View/Canvas/Tools/GCanvasTool.hpp"
#include <QVector>
namespace totcad {
class GInstanceAnnotationTool final : public GCanvasTool {
    Q_OBJECT
public:
    explicit GInstanceAnnotationTool(GGraphicsView *view, QObject *parent = nullptr);
    bool mousePress(QMouseEvent *event) override;
    bool keyPress(QKeyEvent *event) override;
signals:
    void assignmentRequested(const QVector<EntityID> &entityIds);
};
} // namespace totcad
