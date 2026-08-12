#include "Model/Annotation/GAnnotationDocument.hpp"

#include <QUuid>

namespace totcad {

namespace {
QString newId()
{
    return QUuid::createUuid().toString(QUuid::WithoutBraces);
}
}

GAnnotationDocument::GAnnotationDocument(QObject *parent) : QObject(parent) {}

int GAnnotationDocument::typeIndex(const QString &id) const
{
    for (int i = 0; i < m_types.size(); ++i)
        if (m_types.at(i).id == id)
            return i;
    return -1;
}

int GAnnotationDocument::instanceIndex(const QString &id) const
{
    for (int i = 0; i < m_instances.size(); ++i)
        if (m_instances.at(i).id == id)
            return i;
    return -1;
}

const GAnnotationType *GAnnotationDocument::type(const QString &id) const
{
    const int index = typeIndex(id);
    return index < 0 ? nullptr : &m_types.at(index);
}

const GAnnotationInstance *GAnnotationDocument::instance(const QString &id) const
{
    const int index = instanceIndex(id);
    return index < 0 ? nullptr : &m_instances.at(index);
}

QVector<GAnnotationInstance> GAnnotationDocument::instancesForType(const QString &typeId) const
{
    QVector<GAnnotationInstance> result;
    for (const auto &value : m_instances)
        if (value.typeId == typeId)
            result.append(value);
    return result;
}

int GAnnotationDocument::assignedTypeEntityCount(const QString &typeId) const
{
    if (typeId.isEmpty())
        return m_entityTypes.size();
    int count = 0;
    for (auto it = m_entityTypes.cbegin(); it != m_entityTypes.cend(); ++it)
        count += it.value() == typeId ? 1 : 0;
    return count;
}

int GAnnotationDocument::assignedInstanceEntityCount(const QString &instanceId) const
{
    if (instanceId.isEmpty())
        return m_entityInstances.size();
    const GAnnotationInstance *value = instance(instanceId);
    return value ? value->entityIds.size() : 0;
}

QString GAnnotationDocument::addType(QString name, QColor color)
{
    GAnnotationType value;
    value.id = newId();
    value.name = name.isEmpty() ? tr("类型%1").arg(m_types.size() + 1) : std::move(name);
    value.color = color.isValid() ? color : QColor::fromHsv((m_types.size() * 67) % 360, 210, 245);
    insertType(m_types.size(), value);
    return value.id;
}

void GAnnotationDocument::insertType(int index, GAnnotationType typeValue)
{
    if (typeValue.id.isEmpty())
        typeValue.id = newId();
    index = qBound(0, index, m_types.size());
    m_types.insert(index, std::move(typeValue));
    markDirty();
    emit typesChanged();
}

bool GAnnotationDocument::removeType(const QString &id)
{
    const int index = typeIndex(id);
    if (index < 0)
        return false;

    QStringList instanceIds;
    for (const auto &value : m_instances)
        if (value.typeId == id)
            instanceIds.append(value.id);
    for (const QString &instanceId : instanceIds)
        removeInstance(instanceId);

    for (auto it = m_entityTypes.begin(); it != m_entityTypes.end();) {
        if (it.value() == id)
            it = m_entityTypes.erase(it);
        else
            ++it;
    }
    m_types.removeAt(index);
    markDirty();
    emit typesChanged();
    emit assignmentsChanged();
    return true;
}

bool GAnnotationDocument::moveType(const QString &id, int offset)
{
    const int from = typeIndex(id);
    const int to = from + offset;
    if (from < 0 || to < 0 || to >= m_types.size())
        return false;
    m_types.move(from, to);
    markDirty();
    emit typesChanged();
    return true;
}

bool GAnnotationDocument::setTypeName(const QString &id, const QString &name)
{
    const int index = typeIndex(id);
    if (index < 0 || name.trimmed().isEmpty())
        return false;
    m_types[index].name = name.trimmed();
    markDirty();
    emit typesChanged();
    return true;
}

bool GAnnotationDocument::setTypeColor(const QString &id, const QColor &color)
{
    const int index = typeIndex(id);
    if (index < 0 || !color.isValid())
        return false;
    m_types[index].color = color;
    markDirty();
    emit typesChanged();
    emit assignmentsChanged();
    return true;
}

QString GAnnotationDocument::addInstance(const QString &typeId, QString name)
{
    if (!type(typeId))
        return {};
    GAnnotationInstance value;
    value.id = newId();
    value.typeId = typeId;
    const int sequence = instancesForType(typeId).size() + 1;
    value.name = name.isEmpty() ? tr("实例%1").arg(sequence) : std::move(name);
    insertInstance(m_instances.size(), value);
    return value.id;
}

void GAnnotationDocument::insertInstance(int index, GAnnotationInstance value)
{
    if (!type(value.typeId))
        return;
    if (value.id.isEmpty())
        value.id = newId();
    index = qBound(0, index, m_instances.size());
    m_instances.insert(index, std::move(value));
    markDirty();
    emit instancesChanged();
}

bool GAnnotationDocument::removeInstance(const QString &id)
{
    const int index = instanceIndex(id);
    if (index < 0)
        return false;
    for (EntityID entityId : m_instances.at(index).entityIds)
        m_entityInstances.remove(entityId);
    m_instances.removeAt(index);
    markDirty();
    emit instancesChanged();
    emit assignmentsChanged();
    return true;
}

bool GAnnotationDocument::setInstanceName(const QString &id, const QString &name)
{
    const int index = instanceIndex(id);
    if (index < 0 || name.trimmed().isEmpty())
        return false;
    m_instances[index].name = name.trimmed();
    markDirty();
    emit instancesChanged();
    return true;
}

void GAnnotationDocument::assignType(const QVector<EntityID> &entityIds, const QString &typeId)
{
    if (!type(typeId) || entityIds.isEmpty())
        return;
    for (EntityID entityId : entityIds) {
        m_entityTypes.insert(entityId, typeId);
        const QString currentInstanceId = m_entityInstances.value(entityId);
        const GAnnotationInstance *currentInstance = instance(currentInstanceId);
        if (currentInstance && currentInstance->typeId != typeId) {
            const int index = instanceIndex(currentInstanceId);
            m_instances[index].entityIds.remove(entityId);
            m_entityInstances.remove(entityId);
        }
    }
    markDirty();
    emit instancesChanged();
    emit assignmentsChanged();
}

void GAnnotationDocument::assignInstance(const QVector<EntityID> &entityIds, const QString &instanceId)
{
    const int targetIndex = instanceIndex(instanceId);
    if (targetIndex < 0 || entityIds.isEmpty())
        return;
    const QString targetType = m_instances.at(targetIndex).typeId;
    for (EntityID entityId : entityIds) {
        const QString oldInstanceId = m_entityInstances.value(entityId);
        const int oldIndex = instanceIndex(oldInstanceId);
        if (oldIndex >= 0)
            m_instances[oldIndex].entityIds.remove(entityId);
        m_instances[targetIndex].entityIds.insert(entityId);
        m_entityInstances.insert(entityId, instanceId);
        m_entityTypes.insert(entityId, targetType);
    }
    markDirty();
    emit instancesChanged();
    emit assignmentsChanged();
}

void GAnnotationDocument::clearAssignments(const QVector<EntityID> &entityIds)
{
    for (EntityID entityId : entityIds) {
        const int index = instanceIndex(m_entityInstances.take(entityId));
        if (index >= 0)
            m_instances[index].entityIds.remove(entityId);
        m_entityTypes.remove(entityId);
    }
    markDirty();
    emit instancesChanged();
    emit assignmentsChanged();
}

GAnnotationSnapshot GAnnotationDocument::snapshot() const
{
    return {m_types, m_instances, m_entityTypes, m_entityInstances};
}

void GAnnotationDocument::restore(const GAnnotationSnapshot &value, bool markDocumentDirty)
{
    m_types = value.types;
    m_instances = value.instances;
    m_entityTypes = value.entityTypes;
    m_entityInstances = value.entityInstances;
    if (markDocumentDirty)
        markDirty();
    emit documentReset();
    emit typesChanged();
    emit instancesChanged();
    emit assignmentsChanged();
}

void GAnnotationDocument::clear()
{
    m_types.clear();
    m_instances.clear();
    m_entityTypes.clear();
    m_entityInstances.clear();
    m_dirty = false;
    emit documentReset();
    emit dirtyChanged(false);
}

void GAnnotationDocument::setClean()
{
    if (!m_dirty)
        return;
    m_dirty = false;
    emit dirtyChanged(false);
}

void GAnnotationDocument::markDirty()
{
    if (m_dirty)
        return;
    m_dirty = true;
    emit dirtyChanged(true);
}

} // namespace totcad
