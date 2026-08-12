#pragma once
#include "Model/Entities/GEntity.hpp"
#include <QObject>
#include <QVector>
namespace totcad {
class GScene;
class GSelectionController final : public QObject {
    Q_OBJECT
public:
    explicit GSelectionController(GScene *scene, QObject *parent = nullptr);
    QVector<EntityID> selectedEntityIds() const;
signals:
    void selectionChanged(const QVector<EntityID> &entityIds);
private:
    GScene *m_scene;
};
} // namespace totcad
