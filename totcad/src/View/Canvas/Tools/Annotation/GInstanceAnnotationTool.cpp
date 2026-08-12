#include "View/Canvas/Tools/Annotation/GInstanceAnnotationTool.hpp"
#include "View/Canvas/GGraphicsScene.hpp"
#include "View/Canvas/GGraphicsView.hpp"
#include <QKeyEvent>
#include <QMouseEvent>
namespace totcad {
GInstanceAnnotationTool::GInstanceAnnotationTool(GGraphicsView *view, QObject *parent) : GCanvasTool(view, parent) {}
bool GInstanceAnnotationTool::mousePress(QMouseEvent *event)
{
    if (event->button() != Qt::RightButton) return false;
    auto *cadScene = qobject_cast<GGraphicsScene *>(view()->scene());
    const QVector<EntityID> ids = cadScene ? cadScene->selectedEntityIds() : QVector<EntityID>{};
    if (ids.isEmpty()) emit canceled(); else emit assignmentRequested(ids);
    if (cadScene) cadScene->clearSelection();
    event->accept();
    return true;
}
bool GInstanceAnnotationTool::keyPress(QKeyEvent *event)
{
    if (event->key() != Qt::Key_Escape) return false;
    auto *cadScene = qobject_cast<GGraphicsScene *>(view()->scene());
    if (cadScene && !cadScene->selectedItems().isEmpty()) cadScene->clearSelection(); else emit canceled();
    event->accept();
    return true;
}
} // namespace totcad
