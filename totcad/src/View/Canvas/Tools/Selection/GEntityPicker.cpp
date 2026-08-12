#include "View/Canvas/Tools/Selection/GEntityPicker.hpp"
#include "View/Canvas/GGraphicsScene.hpp"
namespace totcad {
QVector<EntityID> GEntityPicker::selectedEntities(const GGraphicsScene &scene) { return scene.selectedEntityIds(); }
} // namespace totcad
