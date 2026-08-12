#include "View/CAD/GCADScene.hpp"

#include "Model/Annotation/GAnnotationDocument.hpp"
#include "Model/CAD/GCADArc.hpp"
#include "Model/CAD/GCADCircle.hpp"
#include "Model/CAD/GCADDimension.hpp"
#include "Model/CAD/GCADDocument.hpp"
#include "Model/CAD/GCADHatch.hpp"
#include "Model/CAD/GCADInsert.hpp"
#include "Model/CAD/GCADLine.hpp"
#include "Model/CAD/GCADPoint.hpp"
#include "Model/CAD/GCADPolyline.hpp"
#include "Model/CAD/GCADText.hpp"
#include "View/CAD/GCADEntityItem.hpp"
#include "View/CAD/GInstanceBoxItem.hpp"

#include <QFont>
#include <QPainterPath>
#include <QSet>
#include <QtMath>

namespace totcad {

namespace {
QPointF scenePoint(const QPointF &point) { return {point.x(), -point.y()}; }

QColor aciColor(int colorIndex)
{
    switch (qAbs(colorIndex)) {
    case 1: return Qt::red;
    case 2: return Qt::yellow;
    case 3: return Qt::green;
    case 4: return Qt::cyan;
    case 5: return Qt::blue;
    case 6: return Qt::magenta;
    case 7: return Qt::white;
    case 8: return QColor(128, 128, 128);
    case 9: return QColor(192, 192, 192);
    default: return QColor::fromHsv(((qAbs(colorIndex) - 10) * 17) % 360, 180, 230);
    }
}

double positiveSweep(double start, double end)
{
    double result = end - start;
    while (result <= 0.0)
        result += 360.0;
    return result;
}
}

GCADScene::GCADScene(QObject *parent) : QGraphicsScene(parent)
{
    setBackgroundBrush(QColor(25, 27, 31));
}

void GCADScene::setDocuments(GCADDocument *cadDocument, GAnnotationDocument *annotationDocument)
{
    m_cadDocument = cadDocument;
    m_annotationDocument = annotationDocument;
    if (m_cadDocument)
        connect(m_cadDocument, &GCADDocument::layerVisibilityChanged,
                this, &GCADScene::rebuild, Qt::UniqueConnection);
    if (m_annotationDocument) {
        connect(m_annotationDocument, &GAnnotationDocument::assignmentsChanged,
                this, &GCADScene::refreshStyles, Qt::UniqueConnection);
        connect(m_annotationDocument, &GAnnotationDocument::instancesChanged,
                this, &GCADScene::refreshInstanceBoxes, Qt::UniqueConnection);
        connect(m_annotationDocument, &GAnnotationDocument::documentReset,
                this, &GCADScene::refreshStyles, Qt::UniqueConnection);
    }
    rebuild();
}

void GCADScene::rebuild()
{
    clear();
    m_entityItems.clear();
    m_instanceBoxes.clear();
    if (!m_cadDocument)
        return;

    for (const auto &entity : m_cadDocument->entities()) {
        if (!entity->paperSpace)
            addEntity(entity, entity->id(), QTransform{});
    }
    refreshInstanceBoxes();
    const QRectF contents = itemsBoundingRect();
    setSceneRect(contents.isEmpty() ? QRectF(-100.0, -100.0, 200.0, 200.0)
                                          : contents.adjusted(-10.0, -10.0, 10.0, 10.0));
}

void GCADScene::addEntity(const std::shared_ptr<GCADEntity> &entity,
                          const QString &selectionId,
                          const QTransform &transform,
                          int insertionDepth)
{
    if (!entity || insertionDepth > 16)
        return;
    const auto layerIt = m_cadDocument->layers().constFind(entity->layerName);
    if (layerIt != m_cadDocument->layers().cend() && !layerIt->visible)
        return;

    if (entity->type() == GCADEntityType::Insert) {
        const auto insert = std::static_pointer_cast<GCADInsert>(entity);
        const GCADBlock *block = m_cadDocument->block(insert->blockName);
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

    QPainterPath path;
    bool filled = false;
    switch (entity->type()) {
    case GCADEntityType::Point: {
        const auto value = std::static_pointer_cast<GCADPoint>(entity);
        path.addEllipse(scenePoint(value->position), 1.5, 1.5);
        filled = true;
        break;
    }
    case GCADEntityType::Line: {
        const auto value = std::static_pointer_cast<GCADLine>(entity);
        path.moveTo(scenePoint(value->start));
        path.lineTo(scenePoint(value->end));
        break;
    }
    case GCADEntityType::Circle: {
        const auto value = std::static_pointer_cast<GCADCircle>(entity);
        path.addEllipse(QRectF(value->center.x() - value->radius,
                              -value->center.y() - value->radius,
                              value->radius * 2.0,
                              value->radius * 2.0));
        break;
    }
    case GCADEntityType::Arc: {
        const auto value = std::static_pointer_cast<GCADArc>(entity);
        const QRectF rectangle(value->center.x() - value->radius,
                               -value->center.y() - value->radius,
                               value->radius * 2.0,
                               value->radius * 2.0);
        path.arcMoveTo(rectangle, -value->startAngle);
        path.arcTo(rectangle, -value->startAngle, -positiveSweep(value->startAngle, value->endAngle));
        break;
    }
    case GCADEntityType::Polyline: {
        const auto value = std::static_pointer_cast<GCADPolyline>(entity);
        if (!value->vertices.isEmpty()) {
            path.moveTo(scenePoint(value->vertices.first()));
            for (int i = 1; i < value->vertices.size(); ++i)
                path.lineTo(scenePoint(value->vertices.at(i)));
            if (value->closed)
                path.closeSubpath();
        }
        break;
    }
    case GCADEntityType::Text: {
        const auto value = std::static_pointer_cast<GCADText>(entity);
        QFont font;
        font.setPointSizeF(qMax(1.0, value->height));
        QPainterPath textPath;
        textPath.addText(scenePoint(value->position), font, value->text);
        QTransform rotation;
        rotation.translate(value->position.x(), -value->position.y());
        rotation.rotate(-value->rotation);
        rotation.translate(-value->position.x(), value->position.y());
        path = rotation.map(textPath);
        filled = true;
        break;
    }
    case GCADEntityType::Dimension: {
        const auto value = std::static_pointer_cast<GCADDimension>(entity);
        path.moveTo(scenePoint(value->definitionPoint));
        path.lineTo(scenePoint(value->textPoint));
        if (!value->text.isEmpty()) {
            QFont font;
            font.setPointSizeF(2.5);
            path.addText(scenePoint(value->textPoint), font, value->text);
        }
        break;
    }
    case GCADEntityType::Hatch: {
        const auto value = std::static_pointer_cast<GCADHatch>(entity);
        path.setFillRule(Qt::OddEvenFill);
        for (const QPolygonF &boundary : value->boundaries) {
            QPolygonF mapped;
            for (const QPointF &point : boundary)
                mapped.append(scenePoint(point));
            path.addPolygon(mapped);
        }
        filled = value->solid;
        break;
    }
    case GCADEntityType::Insert:
        return;
    }

    if (path.isEmpty())
        return;
    auto *item = new GCADEntityItem(selectionId, entity->layerName, path);
    const QColor color = entityColor(*entity, selectionId);
    item->setBaseStyle(QPen(color, 0.0), filled ? QBrush(color) : Qt::NoBrush);
    item->setTransform(transform);
    addItem(item);
    m_entityItems[selectionId].append(item);
}

QColor GCADScene::entityColor(const GCADEntity &entity, const QString &selectionId) const
{
    if (m_annotationDocument) {
        const QString typeId = m_annotationDocument->typeForEntity(selectionId);
        if (const GAnnotationType *type = m_annotationDocument->type(typeId))
            return type->color;
    }
    if (entity.trueColor.isValid())
        return entity.trueColor;
    if (entity.colorIndex > 0 && entity.colorIndex < 256)
        return aciColor(entity.colorIndex);
    if (m_cadDocument) {
        const auto it = m_cadDocument->layers().constFind(entity.layerName);
        if (it != m_cadDocument->layers().cend())
            return it->color;
    }
    return Qt::white;
}

void GCADScene::refreshStyles()
{
    if (!m_cadDocument)
        return;
    for (auto it = m_entityItems.begin(); it != m_entityItems.end(); ++it) {
        const auto entity = m_cadDocument->entity(it.key());
        if (!entity)
            continue;
        const QColor color = entityColor(*entity, it.key());
        for (GCADEntityItem *item : it.value())
            item->setBaseStyle(QPen(color, 0.0), item->brush().style() == Qt::NoBrush ? Qt::NoBrush : QBrush(color));
    }
    refreshInstanceBoxes();
}

QRectF GCADScene::instanceBounds(const QString &instanceId) const
{
    if (!m_annotationDocument)
        return {};
    const GAnnotationInstance *instance = m_annotationDocument->instance(instanceId);
    if (!instance)
        return {};
    QRectF result;
    for (const QString &entityId : instance->entityIds) {
        for (GCADEntityItem *item : m_entityItems.value(entityId))
            result = result.isNull() ? item->sceneBoundingRect() : result.united(item->sceneBoundingRect());
    }
    return result;
}

void GCADScene::refreshInstanceBoxes()
{
    for (GInstanceBoxItem *item : m_instanceBoxes)
        delete item;
    m_instanceBoxes.clear();
    if (!m_annotationDocument)
        return;
    for (const GAnnotationInstance &instance : m_annotationDocument->instances()) {
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

QStringList GCADScene::selectedEntityIds() const
{
    QSet<QString> ids;
    for (QGraphicsItem *item : selectedItems()) {
        if (auto *entityItem = dynamic_cast<GCADEntityItem *>(item))
            ids.insert(entityItem->entityId());
    }
    return ids.values();
}

} // namespace totcad
