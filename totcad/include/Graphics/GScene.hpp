#pragma once

#include "Model/Entities/GEntity.hpp"

#include <QGraphicsScene>
#include <QHash>
#include <QList>
#include <QPen>
#include <memory>

namespace totcad {

class GAnnotationDocument;
class GDocumentEntity;
class GEntityItem;
class GInstanceBoxItem;

class GScene final : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit GScene(QObject *parent = nullptr);

    void setDocuments(GDocumentEntity *document, GAnnotationDocument *annotationDocument);
    void rebuild();
    void refreshStyles();
    void refreshInstanceBoxes();
    QVector<EntityID> selectedEntityIds() const;
    QRectF instanceBounds(const QString &instanceId) const;

private:
    void addEntity(const std::shared_ptr<GEntity> &entity,
                   EntityID selectionId,
                   const QTransform &transform,
                   int insertionDepth = 0);
    QColor entityColor(const GEntity &entity, EntityID selectionId) const;
    QPen entityPen(const GEntity &entity, EntityID selectionId) const;

    GDocumentEntity *m_document{nullptr};
    GAnnotationDocument *m_annotationDocument{nullptr};
    QHash<EntityID, QList<GEntityItem *>> m_entityItems;
    QHash<QString, GInstanceBoxItem *> m_instanceBoxes;
};

} // namespace totcad
