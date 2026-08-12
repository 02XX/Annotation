#include "IO/DXF/GDXFLoader.hpp"

#include "IO/DXF/Reader.hpp"
#include "Model/Entities/GArcEntity.hpp"
#include "Model/Entities/GCircleEntity.hpp"
#include "Model/Entities/GDimensionEntity.hpp"
#include "Model/Entities/GHatchEntity.hpp"
#include "Model/Entities/GInsertEntity.hpp"
#include "Model/Entities/GLineEntity.hpp"
#include "Model/Entities/GPointEntity.hpp"
#include "Model/Entities/GPolylineEntity.hpp"
#include "Model/Entities/GTextEntity.hpp"
#include "Model/GDXFModel.hpp"

#include <QColor>
#include <QVector>

#include <charconv>
#include <cmath>
#include <memory>
#include <optional>
#include <string>
#include <unordered_set>
#include <utility>

namespace totcad
{

namespace
{

using dxf::Record;

QString text(const std::optional<std::string> &value, const char *fallback = "")
{
    return QString::fromUtf8(value.value_or(fallback).c_str());
}

double number(const Record &record, int code, double fallback = 0.0)
{
    return record.doubleValue(code).value_or(fallback);
}

int integer(const Record &record, int code, int fallback = 0)
{
    return record.intValue(code).value_or(fallback);
}

QPointF point(const Record &record, int xCode)
{
    return {number(record, xCode), number(record, xCode + 10)};
}

QColor aciColor(int colorIndex)
{
    switch (std::abs(colorIndex))
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
    default:
        return QColor::fromHsv(((std::abs(colorIndex) - 10) * 17) % 360, 180, 230);
    }
}

class IdSource final
{
  public:
    EntityID idFor(const Record &record)
    {
        EntityID candidate = 0;
        if (const auto handle = record.stringValue(5))
        {
            const char *begin = handle->data();
            const char *end = begin + handle->size();
            const auto parsed = std::from_chars(begin, end, candidate, 16);
            if (parsed.ec != std::errc{} || parsed.ptr != end)
                candidate = 0;
        }
        if (candidate == 0 || m_used.count(candidate) != 0)
        {
            while (m_used.count(m_next) != 0)
                ++m_next;
            candidate = m_next++;
        }
        m_used.insert(candidate);
        return candidate;
    }

  private:
    EntityID m_next{1};
    std::unordered_set<EntityID> m_used;
};

void setCommonFields(const Record &record, GEntity &entity)
{
    entity.layerName = text(record.stringValue(8), "0");
    entity.lineTypeName = text(record.stringValue(6), "BYLAYER");
    entity.colorIndex = integer(record, 62, 256);
    entity.paperSpace = integer(record, 67) == 1;
    if (const auto trueColor = record.intValue(420))
        entity.trueColor = QColor::fromRgb(static_cast<QRgb>(*trueColor));
}

std::shared_ptr<GEntity> makeSimpleEntity(const Record &record, IdSource &ids)
{
    std::shared_ptr<GEntity> entity;
    const EntityID id = ids.idFor(record);

    if (record.type == "POINT")
    {
        auto value = std::make_shared<GPointEntity>(id);
        value->position = point(record, 10);
        entity = std::move(value);
    }
    else if (record.type == "LINE")
    {
        auto value = std::make_shared<GLineEntity>(id);
        value->start = point(record, 10);
        value->end = point(record, 11);
        entity = std::move(value);
    }
    else if (record.type == "CIRCLE")
    {
        auto value = std::make_shared<GCircleEntity>(id);
        value->center = point(record, 10);
        value->radius = number(record, 40);
        entity = std::move(value);
    }
    else if (record.type == "ARC")
    {
        auto value = std::make_shared<GArcEntity>(id);
        value->center = point(record, 10);
        value->radius = number(record, 40);
        value->startAngle = number(record, 50);
        value->endAngle = number(record, 51);
        entity = std::move(value);
    }
    else if (record.type == "TEXT" || record.type == "MTEXT")
    {
        auto value = std::make_shared<GTextEntity>(id);
        value->position = point(record, 10);
        value->text = text(record.stringValue(1));
        value->height = number(record, 40, 2.5);
        value->rotation = number(record, 50);
        entity = std::move(value);
    }
    else if (record.type == "INSERT")
    {
        auto value = std::make_shared<GInsertEntity>(id);
        value->blockName = text(record.stringValue(2));
        value->position = point(record, 10);
        value->scaleX = number(record, 41, 1.0);
        value->scaleY = number(record, 42, 1.0);
        value->rotation = number(record, 50);
        entity = std::move(value);
    }
    else if (record.type == "DIMENSION")
    {
        auto value = std::make_shared<GDimensionEntity>(id);
        value->blockName = text(record.stringValue(2));
        value->text = text(record.stringValue(1));
        value->definitionPoint = point(record, 10);
        value->textPoint = point(record, 11);
        entity = std::move(value);
    }

    if (entity)
        setCommonFields(record, *entity);
    return entity;
}

std::shared_ptr<GPolylineEntity> makeLightweightPolyline(const Record &record, IdSource &ids)
{
    auto entity = std::make_shared<GPolylineEntity>(ids.idFor(record));
    setCommonFields(record, *entity);
    entity->closed = (integer(record, 70) & 1) != 0;

    QPointF vertex;
    double bulge = 0.0;
    bool hasVertex = false;
    const auto appendVertex = [&] {
        if (!hasVertex)
            return;
        entity->vertices.push_back(vertex);
        entity->bulges.push_back(bulge);
    };

    for (const dxf::Group &group : record.groups)
    {
        if (group.code == 10)
        {
            appendVertex();
            vertex = {group.toDouble().value_or(0.0), 0.0};
            bulge = 0.0;
            hasVertex = true;
        }
        else if (group.code == 20 && hasVertex)
        {
            vertex.setY(group.toDouble().value_or(0.0));
        }
        else if (group.code == 42 && hasVertex)
        {
            bulge = group.toDouble().value_or(0.0);
        }
    }
    appendVertex();
    return entity;
}

std::shared_ptr<GPolylineEntity> makePolyline(const Record &header, const std::vector<Record> &records,
                                              std::size_t &index, IdSource &ids)
{
    auto entity = std::make_shared<GPolylineEntity>(ids.idFor(header));
    setCommonFields(header, *entity);
    entity->closed = (integer(header, 70) & 1) != 0;

    for (++index; index < records.size() && records[index].type != "SEQEND"; ++index)
    {
        if (records[index].type != "VERTEX")
            continue;
        entity->vertices.push_back(point(records[index], 10));
        entity->bulges.push_back(number(records[index], 42));
    }
    return entity;
}

std::shared_ptr<GHatchEntity> makeHatch(const Record &record, IdSource &ids)
{
    auto entity = std::make_shared<GHatchEntity>(ids.idFor(record));
    setCommonFields(record, *entity);
    entity->patternName = text(record.stringValue(2), "SOLID");
    entity->solid = integer(record, 70, 1) != 0;

    QPolygonF boundary;
    QPointF vertex;
    bool hasVertex = false;
    const auto appendVertex = [&] {
        if (hasVertex)
            boundary.push_back(vertex);
    };
    for (const dxf::Group &group : record.groups)
    {
        if (group.code == 10)
        {
            appendVertex();
            vertex = {group.toDouble().value_or(0.0), 0.0};
            hasVertex = true;
        }
        else if (group.code == 20 && hasVertex)
        {
            vertex.setY(group.toDouble().value_or(0.0));
        }
    }
    appendVertex();
    if (!boundary.isEmpty())
        entity->boundaries.push_back(std::move(boundary));
    return entity;
}

QVector<std::shared_ptr<GEntity>> makeEntities(const std::vector<Record> &records, IdSource &ids)
{
    QVector<std::shared_ptr<GEntity>> entities;
    for (std::size_t index = 0; index < records.size(); ++index)
    {
        std::shared_ptr<GEntity> entity;
        if (records[index].type == "LWPOLYLINE")
        {
            entity = makeLightweightPolyline(records[index], ids);
        }
        else if (records[index].type == "POLYLINE")
        {
            entity = makePolyline(records[index], records, index, ids);
        }
        else if (records[index].type == "HATCH")
        {
            entity = makeHatch(records[index], ids);
        }
        else
        {
            entity = makeSimpleEntity(records[index], ids);
        }
        if (entity)
            entities.push_back(std::move(entity));
    }
    return entities;
}

void importTables(const dxf::DXFDocument &source, GDXFModel &target)
{
    if (const dxf::Table *table = source.findTable("LTYPE"))
    {
        for (const Record &record : table->records)
        {
            GLineTypeEntity lineType;
            lineType.name = text(record.stringValue(2));
            lineType.description = text(record.stringValue(3));
            for (const dxf::Group &group : record.groups)
            {
                if (group.code == 49)
                    lineType.pattern.push_back(group.toDouble().value_or(0.0));
            }
            if (!lineType.name.isEmpty())
                target.addLineType(std::move(lineType));
        }
    }

    if (const dxf::Table *table = source.findTable("LAYER"))
    {
        for (const Record &record : table->records)
        {
            GLayerEntity layer;
            layer.name = text(record.stringValue(2));
            layer.colorIndex = integer(record, 62, 7);
            layer.color = aciColor(layer.colorIndex);
            layer.lineTypeName = text(record.stringValue(6), "CONTINUOUS");
            layer.lineWidth = number(record, 370, 0.0) / 100.0;
            layer.visible = layer.colorIndex >= 0 && (integer(record, 70) & 1) == 0;
            if (!layer.name.isEmpty())
                target.addLayer(std::move(layer));
        }
    }
}

void importBlocks(const dxf::DXFDocument &source, GDXFModel &target, IdSource &ids)
{
    for (const dxf::Block &sourceBlock : source.blocks)
    {
        GBlockEntity block;
        block.name = text(sourceBlock.definition.stringValue(2));
        block.basePoint = point(sourceBlock.definition, 10);
        block.entities = makeEntities(sourceBlock.entities, ids);
        if (!block.name.isEmpty())
            target.addBlock(std::move(block));
    }
}

} // namespace

bool GDXFLoader::load(const std::string &filePath, GDXFModel &drawing, std::string *errorMessage) const
{
    dxf::DXFDocument source;
    std::string error;
    if (!dxf::Reader{}.readFile(std::filesystem::u8path(filePath), source, &error))
    {
        if (errorMessage)
            *errorMessage = error;
        return false;
    }

    GDXFModel parsed;
    IdSource ids;
    importTables(source, parsed);
    importBlocks(source, parsed, ids);
    for (auto &entity : makeEntities(source.entities, ids))
        parsed.addEntity(std::move(entity));
    parsed.setSourcePath(QString::fromUtf8(filePath.c_str()));
    drawing.replaceWith(std::move(parsed));
    if (errorMessage)
        errorMessage->clear();
    return true;
}

} // namespace totcad
