#include "DXF/GDXFParser.hpp"

#include "DXF/GDXFEntityParser.hpp"
#include "DXF/GDXFReader.hpp"
#include "Model/Entities/GDocumentEntity.hpp"
#include "Model/Entities/GPolylineEntity.hpp"

#include <QtMath>

namespace totcad {

namespace {
using Record = QPair<QString, QVector<GDXFGroup>>;

QVector<Record> records(const QVector<GDXFGroup> &groups, int begin, int end)
{
    QVector<Record> result;
    int index = begin;
    while (index < end) {
        if (groups.at(index).code != 0) {
            ++index;
            continue;
        }
        const QString name = groups.at(index).value.toUpper();
        const int start = ++index;
        while (index < end && groups.at(index).code != 0)
            ++index;
        QVector<GDXFGroup> values;
        values.reserve(index - start);
        for (int i = start; i < index; ++i)
            values.append(groups.at(i));
        result.append(qMakePair(name, values));
    }
    return result;
}

QString valueOf(const QVector<GDXFGroup> &groups, int code, const QString &fallback = {})
{
    for (const auto &group : groups)
        if (group.code == code)
            return group.value;
    return fallback;
}

int intValueOf(const QVector<GDXFGroup> &groups, int code, int fallback = 0)
{
    for (const auto &group : groups)
        if (group.code == code)
            return group.toInt(fallback);
    return fallback;
}

QColor aciColor(int value)
{
    const int index = qAbs(value);
    switch (index) {
    case 1: return Qt::red;
    case 2: return Qt::yellow;
    case 3: return Qt::green;
    case 4: return Qt::cyan;
    case 5: return Qt::blue;
    case 6: return Qt::magenta;
    case 7: return Qt::white;
    case 8: return QColor(128, 128, 128);
    case 9: return QColor(192, 192, 192);
    default:
        return QColor::fromHsv(((index - 10) * 17) % 360, 190, 230);
    }
}

EntityID stableEntityId(const QString &value)
{
    constexpr EntityID offsetBasis = UINT64_C(14695981039346656037);
    constexpr EntityID prime = UINT64_C(1099511628211);
    EntityID result = offsetBasis;
    const QByteArray bytes = value.toUtf8();
    for (char byte : bytes) {
        result ^= static_cast<unsigned char>(byte);
        result *= prime;
    }
    return result;
}

void parseTables(const QVector<GDXFGroup> &section, GDocumentEntity &document)
{
    for (const Record &record : records(section, 0, section.size())) {
        if (record.first == QLatin1String("LAYER")) {
            GLayerEntity layer;
            layer.name = valueOf(record.second, 2, QStringLiteral("0"));
            layer.colorIndex = intValueOf(record.second, 62, 7);
            layer.visible = layer.colorIndex >= 0;
            layer.color = aciColor(layer.colorIndex);
            const int trueColor = intValueOf(record.second, 420, -1);
            if (trueColor >= 0)
                layer.color = QColor::fromRgb(trueColor);
            layer.lineTypeName = valueOf(record.second, 6, QStringLiteral("CONTINUOUS"));
            layer.lineWidth = static_cast<double>(intValueOf(record.second, 370, 0)) / 100.0;
            document.addLayer(layer);
        } else if (record.first == QLatin1String("LTYPE")) {
            GLineTypeEntity lineType;
            lineType.name = valueOf(record.second, 2, QStringLiteral("CONTINUOUS"));
            lineType.description = valueOf(record.second, 3);
            for (const GDXFGroup &group : record.second)
                if (group.code == 49)
                    lineType.pattern.append(group.toDouble());
            document.addLineType(lineType);
        }
    }
}

void parseEntities(const QVector<Record> &source,
                   std::vector<std::shared_ptr<GEntity>> *blockEntities,
                   GDocumentEntity *document,
                   const QString &idPrefix)
{
    GDXFEntityParser parser;
    int generated = 0;
    for (int i = 0; i < source.size(); ++i) {
        const Record &record = source.at(i);
        if (record.first == QLatin1String("VERTEX") || record.first == QLatin1String("SEQEND"))
            continue;
        const EntityID fallbackId = stableEntityId(idPrefix + QString::number(++generated));
        auto entity = parser.parse(record.first, record.second, fallbackId);
        if (!entity)
            continue;

        if (record.first == QLatin1String("POLYLINE")) {
            auto polyline = std::dynamic_pointer_cast<GPolylineEntity>(entity);
            polyline->vertices.clear();
            polyline->bulges.clear();
            int vertexIndex = i + 1;
            while (vertexIndex < source.size() && source.at(vertexIndex).first == QLatin1String("VERTEX")) {
                const auto &groups = source.at(vertexIndex).second;
                double x = 0.0;
                double y = 0.0;
                double bulge = 0.0;
                for (const auto &group : groups) {
                    if (group.code == 10) x = group.toDouble();
                    else if (group.code == 20) y = group.toDouble();
                    else if (group.code == 42) bulge = group.toDouble();
                }
                polyline->vertices.append({x, y});
                polyline->bulges.append(bulge);
                ++vertexIndex;
            }
            i = vertexIndex - 1;
        }

        if (blockEntities)
            blockEntities->push_back(std::move(entity));
        else if (document)
            document->addEntity(std::move(entity));
    }
}

void parseBlocks(const QVector<GDXFGroup> &section, GDocumentEntity &document)
{
    const QVector<Record> values = records(section, 0, section.size());
    int index = 0;
    while (index < values.size()) {
        if (values.at(index).first != QLatin1String("BLOCK")) {
            ++index;
            continue;
        }
        GBlockEntity block;
        block.name = valueOf(values.at(index).second, 2);
        double x = 0.0;
        double y = 0.0;
        for (const auto &group : values.at(index).second) {
            if (group.code == 10) x = group.toDouble();
            else if (group.code == 20) y = group.toDouble();
        }
        block.basePoint = {x, y};
        const int entityBegin = ++index;
        while (index < values.size() && values.at(index).first != QLatin1String("ENDBLK"))
            ++index;
        QVector<Record> entityRecords;
        for (int i = entityBegin; i < index; ++i)
            entityRecords.append(values.at(i));
        parseEntities(entityRecords, &block.entities, nullptr, QStringLiteral("block:%1:").arg(block.name));
        document.addBlock(std::move(block));
        ++index;
    }
}
}

bool GDXFParser::parseFile(const QString &filePath, GDocumentEntity &document, QString *errorMessage) const
{
    QVector<GDXFGroup> groups;
    if (!GDXFReader{}.readFile(filePath, groups, errorMessage))
        return false;
    if (!parse(groups, document, errorMessage))
        return false;
    document.sourcePath = filePath;
    return true;
}

bool GDXFParser::parse(const QVector<GDXFGroup> &groups, GDocumentEntity &document, QString *errorMessage) const
{
    document.clear();
    bool foundEntities = false;
    int index = 0;
    while (index < groups.size()) {
        if (groups.at(index).code != 0 || groups.at(index).value.toUpper() != QLatin1String("SECTION")) {
            ++index;
            continue;
        }
        if (++index >= groups.size() || groups.at(index).code != 2) {
            if (errorMessage)
                *errorMessage = QObject::tr("DXF SECTION 缺少名称");
            return false;
        }
        const QString sectionName = groups.at(index).value.toUpper();
        const int sectionBegin = ++index;
        while (index < groups.size()
               && !(groups.at(index).code == 0 && groups.at(index).value.toUpper() == QLatin1String("ENDSEC")))
            ++index;
        QVector<GDXFGroup> section;
        for (int i = sectionBegin; i < index; ++i)
            section.append(groups.at(i));
        if (sectionName == QLatin1String("TABLES"))
            parseTables(section, document);
        else if (sectionName == QLatin1String("BLOCKS"))
            parseBlocks(section, document);
        else if (sectionName == QLatin1String("ENTITIES")) {
            parseEntities(records(section, 0, section.size()), nullptr, &document, QStringLiteral("entity:"));
            foundEntities = true;
        }
        ++index;
    }

    if (!foundEntities && document.blocks().isEmpty()) {
        if (errorMessage)
            *errorMessage = QObject::tr("文件中没有可识别的 ENTITIES 或 BLOCKS 段");
        return false;
    }
    return true;
}

} // namespace totcad
