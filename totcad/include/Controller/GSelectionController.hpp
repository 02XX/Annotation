#pragma once
#include <QObject>
namespace totcad {
class GCADScene;
class GSelectionController final : public QObject {
    Q_OBJECT
public:
    explicit GSelectionController(GCADScene *scene, QObject *parent = nullptr);
    QStringList selectedEntityIds() const;
signals:
    void selectionChanged(const QStringList &entityIds);
private:
    GCADScene *m_scene;
};
} // namespace totcad
