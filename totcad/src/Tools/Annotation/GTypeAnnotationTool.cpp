#include "Tools/Annotation/GTypeAnnotationTool.hpp"
#include "Graphics/GScene.hpp"
#include "Graphics/GView.hpp"
#include <QKeyEvent>
#include <QMouseEvent>
namespace totcad {
GTypeAnnotationTool::GTypeAnnotationTool(GView *view, QObject *parent) : GTool(view, parent) {}
bool GTypeAnnotationTool::mousePress(QMouseEvent *event)
{
    if (event->button() != Qt::RightButton)
        return false;
    auto *cadScene = qobject_cast<GScene *>(view()->scene());
    const QVector<EntityID> ids = cadScene ? cadScene->selectedEntityIds() : QVector<EntityID>{};
    if (ids.isEmpty()) emit canceled(); else emit assignmentRequested(ids);
    if (cadScene) cadScene->clearSelection();
    event->accept();
    return true;
}
bool GTypeAnnotationTool::keyPress(QKeyEvent *event)
{
    if (event->key() != Qt::Key_Escape) return false;
    auto *cadScene = qobject_cast<GScene *>(view()->scene());
    if (cadScene && !cadScene->selectedItems().isEmpty()) cadScene->clearSelection(); else emit canceled();
    event->accept();
    return true;
}
} // namespace totcad
