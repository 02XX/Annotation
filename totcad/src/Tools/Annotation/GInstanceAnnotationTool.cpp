#include "Tools/Annotation/GInstanceAnnotationTool.hpp"
#include "Graphics/GCADScene.hpp"
#include "Graphics/GCADView.hpp"
#include <QKeyEvent>
#include <QMouseEvent>
namespace totcad {
GInstanceAnnotationTool::GInstanceAnnotationTool(GCADView *view, QObject *parent) : GCADTool(view, parent) {}
bool GInstanceAnnotationTool::mousePress(QMouseEvent *event)
{
    if (event->button() != Qt::RightButton) return false;
    auto *cadScene = qobject_cast<GCADScene *>(view()->scene());
    const QStringList ids = cadScene ? cadScene->selectedEntityIds() : QStringList{};
    if (ids.isEmpty()) emit canceled(); else emit assignmentRequested(ids);
    if (cadScene) cadScene->clearSelection();
    event->accept();
    return true;
}
bool GInstanceAnnotationTool::keyPress(QKeyEvent *event)
{
    if (event->key() != Qt::Key_Escape) return false;
    auto *cadScene = qobject_cast<GCADScene *>(view()->scene());
    if (cadScene && !cadScene->selectedItems().isEmpty()) cadScene->clearSelection(); else emit canceled();
    event->accept();
    return true;
}
} // namespace totcad
