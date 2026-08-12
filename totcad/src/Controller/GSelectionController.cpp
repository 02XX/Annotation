#include "Controller/GSelectionController.hpp"
#include "Graphics/GCADScene.hpp"
namespace totcad {
GSelectionController::GSelectionController(GCADScene *scene, QObject *parent) : QObject(parent), m_scene(scene)
{
    connect(scene, &QGraphicsScene::selectionChanged, this, [this] { emit selectionChanged(selectedEntityIds()); });
}
QStringList GSelectionController::selectedEntityIds() const { return m_scene->selectedEntityIds(); }
} // namespace totcad
