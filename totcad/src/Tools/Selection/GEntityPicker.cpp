#include "Tools/Selection/GEntityPicker.hpp"
#include "Graphics/GScene.hpp"
namespace totcad {
QVector<EntityID> GEntityPicker::selectedEntities(const GScene &scene) { return scene.selectedEntityIds(); }
} // namespace totcad
