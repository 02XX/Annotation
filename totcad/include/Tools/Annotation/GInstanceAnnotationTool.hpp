#pragma once
#include "Model/Entities/GEntity.hpp"
#include "Tools/GTool.hpp"
#include <QVector>
namespace totcad {
class GInstanceAnnotationTool final : public GTool {
    Q_OBJECT
public:
    explicit GInstanceAnnotationTool(GView *view, QObject *parent = nullptr);
    bool mousePress(QMouseEvent *event) override;
    bool keyPress(QKeyEvent *event) override;
signals:
    void assignmentRequested(const QVector<EntityID> &entityIds);
};
} // namespace totcad
