#pragma once
#include "Model/Entities/GEntity.hpp"
#include <QObject>
#include <QVector>
namespace totcad {
class GGraphicsScene;
class GSelectionController final : public QObject {
    Q_OBJECT
public:
    explicit GSelectionController(GGraphicsScene *scene, QObject *parent = nullptr);
    QVector<EntityID> selectedEntityIds() const;
signals:
    void selectionChanged(const QVector<EntityID> &entityIds);
private:
    GGraphicsScene *m_scene;
};
} // namespace totcad
