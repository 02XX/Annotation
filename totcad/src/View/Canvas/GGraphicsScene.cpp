#include "View/Canvas/GGraphicsScene.hpp"

#include "Model/Entities/GArcEntity.hpp"
#include "Model/Entities/GCircleEntity.hpp"
#include "Model/Entities/GDimensionEntity.hpp"
#include "Model/Entities/GHatchEntity.hpp"
#include "Model/Entities/GInsertEntity.hpp"
#include "Model/Entities/GLineEntity.hpp"
#include "Model/Entities/GPointEntity.hpp"
#include "Model/Entities/GPolylineEntity.hpp"
#include "Model/Entities/GTextEntity.hpp"
#include "Model/GAnnotationModel.hpp"
#include "Model/GDXFModel.hpp"
#include "View/Canvas/Items/GArcItem.hpp"
#include "View/Canvas/Items/GCircleItem.hpp"
#include "View/Canvas/Items/GDimensionItem.hpp"
#include "View/Canvas/Items/GEntityItem.hpp"
#include "View/Canvas/Items/GHatchItem.hpp"
#include "View/Canvas/Items/GInsertItem.hpp"
#include "View/Canvas/Items/GInstanceBoxItem.hpp"
#include "View/Canvas/Items/GLineItem.hpp"
#include "View/Canvas/Items/GPointItem.hpp"
#include "View/Canvas/Items/GPolylineItem.hpp"
#include "View/Canvas/Items/GTextItem.hpp"

#include <QSet>
#include <QtMath>

namespace totcad
{

namespace
{
QColor aciColor(int colorIndex)
{
    switch (qAbs(colorIndex))
    {
    case 1:
        return Qt::red;
    case 2:
        return Qt::yellow;
    case 3:
        return Qt::green;
    case 4:
        return Qt::cyan;
    case 5:
        return Qt::blue;
    case 6:
        return Qt::magenta;
    case 7:
        return Qt::white;
    case 8:
        return QColor(128, 128, 128);
    case 9:
        return QColor(192, 192, 192);
    default:
        return QColor::fromHsv(((qAbs(colorIndex) - 10) * 17) % 360, 180, 230);
    }
}

} // namespace

GGraphicsScene::GGraphicsScene(QObject *parent) : QGraphicsScene(parent)
{
    setBackgroundBrush(QColor(25, 27, 31));
}

void GGraphicsScene::setDocuments(GDXFModel *document, GAnnotationModel *annotationDocument)
{
    m_document = document;
    m_annotationDocument = annotationDocument;
    if (m_document)
    {
        connect(m_document, &GDXFModel::layerVisibilityChanged, this, &GGraphicsScene::rebuild, Qt::UniqueConnection);
        connect(m_document, &GDXFModel::modelReset, this, &GGraphicsScene::rebuild, Qt::UniqueConnection);
    }
    if (m_annotationDocument)
    {
        connect(m_annotationDocument, &GAnnotationModel::assignmentsChanged, this, &GGraphicsScene::refreshStyles,
                Qt::UniqueConnection);
        connect(m_annotationDocument, &GAnnotationModel::instancesChanged, this, &GGraphicsScene::refreshInstanceBoxes,
                Qt::UniqueConnection);
        connect(m_annotationDocument, &GAnnotationModel::annotationReset, this, &GGraphicsScene::refreshStyles,
                Qt::UniqueConnection);
    }
    rebuild();
}

void GGraphicsScene::rebuild()
{
    clear();
    m_entityItems.clear();
    m_instanceBoxes.clear();
    if (!m_document)
        return;

    for (const auto &entity : m_document->entities())
    {
        if (!entity->paperSpace)
            addEntity(entity, entity->id(), QTransform{});
    }
    refreshInstanceBoxes();
    const QRectF contents = itemsBoundingRect();
    setSceneRect(contents.isEmpty() ? QRectF(-100.0, -100.0, 200.0, 200.0)
                                    : contents.adjusted(-10.0, -10.0, 10.0, 10.0));
}

void GGraphicsScene::addEntity(const std::shared_ptr<GEntity> &entity, EntityID selectionId,
                               const QTransform &transform, int insertionDepth)
{
    if (!entity || insertionDepth > 16)
        return;
    const auto layerIt = m_document->layers().find(entity->layerName.toStdString());
    if (layerIt != m_document->layers().end() && !layerIt->second.visible)
        return;

    if (const auto insert = std::dynamic_pointer_cast<GInsertEntity>(entity))
    {
        auto *insertItem = new GInsertItem(insert.get());
        insertItem->setSelectionEntityId(selectionId);
        insertItem->setTransform(transform);
        addItem(insertItem);

        const GBlockEntity *block = m_document->block(insert->blockName.toStdString());
        if (!block)
            return;
        QTransform insertion;
        insertion.translate(insert->position.x(), -insert->position.y());
        insertion.rotate(-insert->rotation);
        insertion.scale(insert->scaleX, insert->scaleY);
        insertion.translate(-block->basePoint.x(), block->basePoint.y());
        const QTransform nested = insertion * transform;
        for (const auto &blockEntity : block->entities)
            addEntity(blockEntity, selectionId, nested, insertionDepth + 1);
        return;
    }

    GEntityItem *item = nullptr;
    bool filled = false;
    if (const auto value = std::dynamic_pointer_cast<GPointEntity>(entity))
    {
        item = new GPointItem(value.get());
        filled = true;
    }
    else if (const auto value = std::dynamic_pointer_cast<GLineEntity>(entity))
    {
        item = new GLineItem(value.get());
    }
    else if (const auto value = std::dynamic_pointer_cast<GCircleEntity>(entity))
    {
        item = new GCircleItem(value.get());
    }
    else if (const auto value = std::dynamic_pointer_cast<GArcEntity>(entity))
    {
        item = new GArcItem(value.get());
    }
    else if (const auto value = std::dynamic_pointer_cast<GPolylineEntity>(entity))
    {
        item = new GPolylineItem(value.get());
    }
    else if (const auto value = std::dynamic_pointer_cast<GTextEntity>(entity))
    {
        item = new GTextItem(value.get());
        filled = true;
    }
    else if (const auto value = std::dynamic_pointer_cast<GDimensionEntity>(entity))
    {
        item = new GDimensionItem(value.get());
    }
    else if (const auto value = std::dynamic_pointer_cast<GHatchEntity>(entity))
    {
        item = new GHatchItem(value.get());
        filled = value->solid;
    }

    if (!item)
        return;
    item->setSelectionEntityId(selectionId);
    auto *graphicsItem = dynamic_cast<QGraphicsItem *>(item);
    if (!graphicsItem)
    {
        delete item;
        return;
    }
    const QColor color = entityColor(*entity, selectionId);
    item->setBaseStyle(entityPen(*entity, selectionId), filled ? QBrush(color) : Qt::NoBrush);
    graphicsItem->setTransform(transform);
    addItem(graphicsItem);
    m_entityItems[selectionId].append(item);
}

QColor GGraphicsScene::entityColor(const GEntity &entity, EntityID selectionId) const
{
    if (m_annotationDocument)
    {
        const QString typeId = m_annotationDocument->typeForEntity(selectionId);
        if (const GAnnotationType *type = m_annotationDocument->type(typeId))
            return type->color;
    }
    if (entity.trueColor.isValid())
        return entity.trueColor;
    if (entity.colorIndex > 0 && entity.colorIndex < 256)
        return aciColor(entity.colorIndex);
    if (m_document)
    {
        const auto it = m_document->layers().find(entity.layerName.toStdString());
        if (it != m_document->layers().end())
            return aciColor(it->second.colorIndex);
    }
    return Qt::white;
}

QPen GGraphicsScene::entityPen(const GEntity &entity, EntityID selectionId) const
{
    QString lineTypeName = entity.lineTypeName;
    double lineWidth = 0.0;
    if (m_document)
    {
        const auto layer = m_document->layers().find(entity.layerName.toStdString());
        if (layer != m_document->layers().end())
        {
            if (lineTypeName.compare(QStringLiteral("BYLAYER"), Qt::CaseInsensitive) == 0)
                lineTypeName = layer->second.lineTypeName;
            lineWidth = layer->second.lineWidth;
        }
    }
    QPen pen(entityColor(entity, selectionId), lineWidth > 0.0 ? qMax(1.0, lineWidth * 2.0) : 0.0);
    pen.setCosmetic(true);
    if (m_document)
    {
        const auto lineType = m_document->lineTypes().find(lineTypeName.toStdString());
        if (lineType != m_document->lineTypes().end() && !lineType->second.pattern.isEmpty())
        {
            QVector<qreal> dashPattern;
            for (double element : lineType->second.pattern)
                dashPattern.append(qMax(0.5, qAbs(element)));
            if (dashPattern.size() % 2 != 0)
                dashPattern += dashPattern;
            pen.setStyle(Qt::CustomDashLine);
            pen.setDashPattern(dashPattern);
        }
    }
    return pen;
}

void GGraphicsScene::refreshStyles()
{
    rebuild();
}

QRectF GGraphicsScene::instanceBounds(const QString &instanceId) const
{
    if (!m_annotationDocument)
        return {};
    const GAnnotationInstance *instance = m_annotationDocument->instance(instanceId);
    if (!instance)
        return {};
    QRectF result;
    for (EntityID entityId : instance->entityIds)
    {
        for (GEntityItem *item : m_entityItems.value(entityId))
        {
            const auto *graphicsItem = dynamic_cast<const QGraphicsItem *>(item);
            if (graphicsItem)
            {
                result = result.isNull() ? graphicsItem->sceneBoundingRect()
                                         : result.united(graphicsItem->sceneBoundingRect());
            }
        }
    }
    return result;
}

void GGraphicsScene::refreshInstanceBoxes()
{
    for (GInstanceBoxItem *item : m_instanceBoxes)
        delete item;
    m_instanceBoxes.clear();
    if (!m_annotationDocument)
        return;
    for (const GAnnotationInstance &instance : m_annotationDocument->instances())
    {
        const QRectF bounds = instanceBounds(instance.id);
        if (bounds.isEmpty())
            continue;
        QColor color = Qt::yellow;
        if (const GAnnotationType *type = m_annotationDocument->type(instance.typeId))
            color = type->color;
        auto *box = new GInstanceBoxItem(instance.id, bounds.adjusted(-1.5, -1.5, 1.5, 1.5), color);
        addItem(box);
        m_instanceBoxes.insert(instance.id, box);
    }
}

QVector<EntityID> GGraphicsScene::selectedEntityIds() const
{
    QSet<EntityID> ids;
    for (QGraphicsItem *item : selectedItems())
    {
        if (auto *entityItem = dynamic_cast<GEntityItem *>(item))
            ids.insert(entityItem->entityId());
    }
    return QVector<EntityID>::fromList(ids.values());
}

} // namespace totcad
