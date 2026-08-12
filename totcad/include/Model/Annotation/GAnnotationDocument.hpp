#pragma once

#include "Model/Annotation/GAnnotationInstance.hpp"
#include "Model/Annotation/GAnnotationType.hpp"

#include <QHash>
#include <QObject>
#include <QVector>

namespace totcad {

struct GAnnotationSnapshot {
    QVector<GAnnotationType> types;
    QVector<GAnnotationInstance> instances;
    QHash<QString, QString> entityTypes;
    QHash<QString, QString> entityInstances;
};

class GAnnotationDocument final : public QObject
{
    Q_OBJECT
public:
    explicit GAnnotationDocument(QObject *parent = nullptr);

    const QVector<GAnnotationType> &types() const noexcept { return m_types; }
    const QVector<GAnnotationInstance> &instances() const noexcept { return m_instances; }
    const QHash<QString, QString> &entityTypes() const noexcept { return m_entityTypes; }
    const QHash<QString, QString> &entityInstances() const noexcept { return m_entityInstances; }

    const GAnnotationType *type(const QString &id) const;
    const GAnnotationInstance *instance(const QString &id) const;
    QVector<GAnnotationInstance> instancesForType(const QString &typeId) const;
    QString typeForEntity(const QString &entityId) const { return m_entityTypes.value(entityId); }
    QString instanceForEntity(const QString &entityId) const { return m_entityInstances.value(entityId); }
    int assignedTypeEntityCount(const QString &typeId = {}) const;
    int assignedInstanceEntityCount(const QString &instanceId = {}) const;

    QString addType(QString name = {}, QColor color = {});
    void insertType(int index, GAnnotationType type);
    bool removeType(const QString &id);
    bool moveType(const QString &id, int offset);
    bool setTypeName(const QString &id, const QString &name);
    bool setTypeColor(const QString &id, const QColor &color);

    QString addInstance(const QString &typeId, QString name = {});
    void insertInstance(int index, GAnnotationInstance instance);
    bool removeInstance(const QString &id);
    bool setInstanceName(const QString &id, const QString &name);

    void assignType(const QStringList &entityIds, const QString &typeId);
    void assignInstance(const QStringList &entityIds, const QString &instanceId);
    void clearAssignments(const QStringList &entityIds);

    GAnnotationSnapshot snapshot() const;
    void restore(const GAnnotationSnapshot &snapshot, bool markDirty = true);
    void clear();
    bool isDirty() const noexcept { return m_dirty; }
    void setClean();

signals:
    void documentReset();
    void typesChanged();
    void instancesChanged();
    void assignmentsChanged();
    void dirtyChanged(bool dirty);

private:
    void markDirty();
    int typeIndex(const QString &id) const;
    int instanceIndex(const QString &id) const;

    QVector<GAnnotationType> m_types;
    QVector<GAnnotationInstance> m_instances;
    QHash<QString, QString> m_entityTypes;
    QHash<QString, QString> m_entityInstances;
    bool m_dirty{false};
};

} // namespace totcad
