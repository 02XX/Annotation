#include "Tools/Selection/GEntityPicker.hpp"
#include "Graphics/GCADScene.hpp"
namespace totcad {
QStringList GEntityPicker::selectedEntities(const GCADScene &scene) { return scene.selectedEntityIds(); }
} // namespace totcad
