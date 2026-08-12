#pragma once
#include <QStringList>
namespace totcad {
class GCADScene;
class GEntityPicker {
public:
    static QStringList selectedEntities(const GCADScene &scene);
};
} // namespace totcad
