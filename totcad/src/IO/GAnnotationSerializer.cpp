#include "IO/GAnnotationSerializer.hpp"

#include "Model/Annotation/GAnnotationDocument.hpp"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSaveFile>

namespace totcad {

namespace {
QString entityIdToString(EntityID entityId)
{
    return QString::number(static_cast<qulonglong>(entityId));
}

EntityID entityIdFromString(const QString &value)
{
    bool ok = false;
    qulonglong result = value.toULongLong(&ok, 10);
    if (!ok)
        result = value.toULongLong(&ok, 16);
    return ok ? static_cast<EntityID>(result) : EntityID{};
}

EntityID entityIdFromJson(const QJsonValue &value)
{
    if (value.isString())
        return entityIdFromString(value.toString());
    return static_cast<EntityID>(value.toVariant().toULongLong());
}
} // namespace

bool GAnnotationSerializer::save(const QString &filePath,
                                const GAnnotationDocument &document,
                                QString *errorMessage) const
{
    QJsonArray types;
    for (const GAnnotationType &type : document.types())
        types.append(QJsonObject{{QStringLiteral("id"), type.id},
                                 {QStringLiteral("name"), type.name},
                                 {QStringLiteral("color"), type.color.name(QColor::HexArgb)}});

    QJsonArray instances;
    for (const GAnnotationInstance &instance : document.instances()) {
        QJsonArray entityIds;
        for (EntityID entityId : instance.entityIds)
            entityIds.append(entityIdToString(entityId));
        instances.append(QJsonObject{{QStringLiteral("id"), instance.id},
                                     {QStringLiteral("typeId"), instance.typeId},
                                     {QStringLiteral("name"), instance.name},
                                     {QStringLiteral("entityIds"), entityIds}});
    }

    QJsonObject entityTypes;
    for (auto it = document.entityTypes().cbegin(); it != document.entityTypes().cend(); ++it)
        entityTypes.insert(entityIdToString(it.key()), it.value());

    QJsonObject root{{QStringLiteral("version"), 1},
                     {QStringLiteral("types"), types},
                     {QStringLiteral("instances"), instances},
                     {QStringLiteral("entityTypes"), entityTypes}};

    QSaveFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        if (errorMessage)
            *errorMessage = QObject::tr("无法写入标注文件：%1").arg(file.errorString());
        return false;
    }
    file.write(QJsonDocument(root).toJson(QJsonDocument::Indented));
    if (!file.commit()) {
        if (errorMessage)
            *errorMessage = QObject::tr("保存标注文件失败：%1").arg(file.errorString());
        return false;
    }
    return true;
}

bool GAnnotationSerializer::load(const QString &filePath,
                                GAnnotationDocument &document,
                                QString *errorMessage) const
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        if (errorMessage)
            *errorMessage = QObject::tr("无法读取标注文件：%1").arg(file.errorString());
        return false;
    }
    QJsonParseError parseError;
    const QJsonDocument json = QJsonDocument::fromJson(file.readAll(), &parseError);
    if (parseError.error != QJsonParseError::NoError || !json.isObject()) {
        if (errorMessage)
            *errorMessage = QObject::tr("标注 JSON 无效：%1").arg(parseError.errorString());
        return false;
    }

    const QJsonObject root = json.object();
    if (root.value(QStringLiteral("version")).toInt() != 1) {
        if (errorMessage)
            *errorMessage = QObject::tr("不支持的标注文件版本");
        return false;
    }

    GAnnotationSnapshot snapshot;
    for (const QJsonValue &item : root.value(QStringLiteral("types")).toArray()) {
        const QJsonObject value = item.toObject();
        GAnnotationType type{value.value(QStringLiteral("id")).toString(),
                            value.value(QStringLiteral("name")).toString(),
                            QColor(value.value(QStringLiteral("color")).toString())};
        if (!type.id.isEmpty() && !type.name.isEmpty() && type.color.isValid())
            snapshot.types.append(type);
    }

    for (const QJsonValue &item : root.value(QStringLiteral("instances")).toArray()) {
        const QJsonObject value = item.toObject();
        GAnnotationInstance instance;
        instance.id = value.value(QStringLiteral("id")).toString();
        instance.typeId = value.value(QStringLiteral("typeId")).toString();
        instance.name = value.value(QStringLiteral("name")).toString();
        for (const QJsonValue &entityIdValue : value.value(QStringLiteral("entityIds")).toArray()) {
            const EntityID entityId = entityIdFromJson(entityIdValue);
            instance.entityIds.insert(entityId);
            snapshot.entityInstances.insert(entityId, instance.id);
        }
        if (!instance.id.isEmpty() && !instance.typeId.isEmpty())
            snapshot.instances.append(instance);
    }

    const QJsonObject entityTypes = root.value(QStringLiteral("entityTypes")).toObject();
    for (auto it = entityTypes.constBegin(); it != entityTypes.constEnd(); ++it)
        snapshot.entityTypes.insert(entityIdFromString(it.key()), it.value().toString());

    document.restore(snapshot, false);
    document.setClean();
    return true;
}

} // namespace totcad
