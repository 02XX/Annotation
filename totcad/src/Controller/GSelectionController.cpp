#include "Controller/GSelectionController.hpp"
#include "Graphics/GScene.hpp"
namespace totcad {
GSelectionController::GSelectionController(GScene *scene, QObject *parent) : QObject(parent), m_scene(scene)
{
    connect(scene, &QGraphicsScene::selectionChanged, this, [this] { emit selectionChanged(selectedEntityIds()); });
}
QVector<EntityID> GSelectionController::selectedEntityIds() const { return m_scene->selectedEntityIds(); }
} // namespace totcad
