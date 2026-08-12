#include "DXF/GDXFEntityParser.hpp"

#include "Model/Entities/GArcEntity.hpp"
#include "Model/Entities/GCircleEntity.hpp"
#include "Model/Entities/GDimensionEntity.hpp"
#include "Model/Entities/GHatchEntity.hpp"
#include "Model/Entities/GInsertEntity.hpp"
#include "Model/Entities/GLineEntity.hpp"
#include "Model/Entities/GPointEntity.hpp"
#include "Model/Entities/GPolylineEntity.hpp"
#include "Model/Entities/GTextEntity.hpp"

namespace totcad {

namespace {
QString stringValue(const QVector<GDXFGroup> &groups, int code, const QString &fallback = {})
{
    for (const GDXFGroup &group : groups)
        if (group.code == code)
            return group.value;
    return fallback;
}

double numberValue(const QVector<GDXFGroup> &groups, int code, double fallback = 0.0)
{
    for (const GDXFGroup &group : groups)
        if (group.code == code)
            return group.toDouble(fallback);
    return fallback;
}

int integerValue(const QVector<GDXFGroup> &groups, int code, int fallback = 0)
{
    for (const GDXFGroup &group : groups)
        if (group.code == code)
            return group.toInt(fallback);
    return fallback;
}

EntityID entityIdValue(const QVector<GDXFGroup> &groups, EntityID fallbackId)
{
    const QString handle = stringValue(groups, 5);
    bool ok = false;
    const EntityID id = static_cast<EntityID>(handle.toULongLong(&ok, 16));
    return ok ? id : fallbackId;
}

void applyCommon(GEntity &entity, const QVector<GDXFGroup> &groups)
{
    entity.layerName = stringValue(groups, 8, QStringLiteral("0"));
    entity.lineTypeName = stringValue(groups, 6, QStringLiteral("BYLAYER"));
    entity.colorIndex = integerValue(groups, 62, 256);
    entity.paperSpace = integerValue(groups, 67, 0) == 1;
    const int rgb = integerValue(groups, 420, -1);
    if (rgb >= 0)
        entity.trueColor = QColor::fromRgb(rgb);
}
}

std::shared_ptr<GEntity> GDXFEntityParser::parse(const QString &entityName,
                                                  const QVector<GDXFGroup> &groups,
                                                  EntityID fallbackId) const
{
    const EntityID entityId = entityIdValue(groups, fallbackId);
    std::shared_ptr<GEntity> result;
    if (entityName == QLatin1String("POINT")) {
        auto value = std::make_shared<GPointEntity>(entityId);
        value->position = {numberValue(groups, 10), numberValue(groups, 20)};
        result = value;
    } else if (entityName == QLatin1String("LINE")) {
        auto value = std::make_shared<GLineEntity>(entityId);
        value->start = {numberValue(groups, 10), numberValue(groups, 20)};
        value->end = {numberValue(groups, 11), numberValue(groups, 21)};
        result = value;
    } else if (entityName == QLatin1String("CIRCLE")) {
        auto value = std::make_shared<GCircleEntity>(entityId);
        value->center = {numberValue(groups, 10), numberValue(groups, 20)};
        value->radius = qAbs(numberValue(groups, 40));
        result = value;
    } else if (entityName == QLatin1String("ARC")) {
        auto value = std::make_shared<GArcEntity>(entityId);
        value->center = {numberValue(groups, 10), numberValue(groups, 20)};
        value->radius = qAbs(numberValue(groups, 40));
        value->startAngle = numberValue(groups, 50);
        value->endAngle = numberValue(groups, 51);
        result = value;
    } else if (entityName == QLatin1String("LWPOLYLINE") || entityName == QLatin1String("POLYLINE")) {
        auto value = std::make_shared<GPolylineEntity>(entityId);
        for (const GDXFGroup &group : groups) {
            if (group.code == 10)
                value->vertices.append(QPointF(group.toDouble(), 0.0));
            else if (group.code == 20 && !value->vertices.isEmpty())
                value->vertices.last().setY(group.toDouble());
            else if (group.code == 42)
                value->bulges.append(group.toDouble());
        }
        value->closed = (integerValue(groups, 70) & 1) != 0;
        result = value;
    } else if (entityName == QLatin1String("TEXT") || entityName == QLatin1String("MTEXT")) {
        auto value = std::make_shared<GTextEntity>(entityId);
        value->position = {numberValue(groups, 10), numberValue(groups, 20)};
        value->height = qMax(0.1, numberValue(groups, 40, 2.5));
        value->rotation = numberValue(groups, 50);
        for (const GDXFGroup &group : groups)
            if (group.code == 1 || group.code == 3)
                value->text += group.value;
        result = value;
    } else if (entityName == QLatin1String("DIMENSION")) {
        auto value = std::make_shared<GDimensionEntity>(entityId);
        value->definitionPoint = {numberValue(groups, 10), numberValue(groups, 20)};
        value->textPoint = {numberValue(groups, 11), numberValue(groups, 21)};
        value->text = stringValue(groups, 1);
        value->blockName = stringValue(groups, 2);
        result = value;
    } else if (entityName == QLatin1String("INSERT")) {
        auto value = std::make_shared<GInsertEntity>(entityId);
        value->blockName = stringValue(groups, 2);
        value->position = {numberValue(groups, 10), numberValue(groups, 20)};
        value->scaleX = numberValue(groups, 41, 1.0);
        value->scaleY = numberValue(groups, 42, 1.0);
        value->rotation = numberValue(groups, 50);
        result = value;
    } else if (entityName == QLatin1String("HATCH")) {
        auto value = std::make_shared<GHatchEntity>(entityId);
        value->patternName = stringValue(groups, 2, QStringLiteral("SOLID"));
        value->solid = integerValue(groups, 70, 1) == 1;
        QPolygonF boundary;
        for (const GDXFGroup &group : groups) {
            if (group.code == 10)
                boundary.append(QPointF(group.toDouble(), 0.0));
            else if (group.code == 20 && !boundary.isEmpty())
                boundary.last().setY(group.toDouble());
        }
        if (!boundary.isEmpty())
            value->boundaries.append(boundary);
        result = value;
    }

    if (result)
        applyCommon(*result, groups);
    return result;
}

} // namespace totcad
