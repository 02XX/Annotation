#pragma once

#include <QGraphicsScene>
#include <QHash>
#include <QList>
#include <memory>

namespace totcad {

class GCADDocument;
class GAnnotationDocument;
class GCADEntity;
class GCADEntityItem;
class GInstanceBoxItem;

class GCADScene final : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit GCADScene(QObject *parent = nullptr);

    void setDocuments(GCADDocument *cadDocument, GAnnotationDocument *annotationDocument);
    void rebuild();
    void refreshStyles();
    void refreshInstanceBoxes();
    QStringList selectedEntityIds() const;
    QRectF instanceBounds(const QString &instanceId) const;

private:
    void addEntity(const std::shared_ptr<GCADEntity> &entity,
                   const QString &selectionId,
                   const QTransform &transform,
                   int insertionDepth = 0);
    QColor entityColor(const GCADEntity &entity, const QString &selectionId) const;

    GCADDocument *m_cadDocument{nullptr};
    GAnnotationDocument *m_annotationDocument{nullptr};
    QHash<QString, QList<GCADEntityItem *>> m_entityItems;
    QHash<QString, GInstanceBoxItem *> m_instanceBoxes;
};

} // namespace totcad
