#pragma once

#include "Model/Entities/GEntity.hpp"

#include <QColor>
#include <QObject>
#include <QVector>

class QUndoStack;

namespace totcad {

class GAnnotationModel;

class GAnnotationController final : public QObject
{
    Q_OBJECT
public:
    GAnnotationController(GAnnotationModel *document, QUndoStack *undoStack, QObject *parent = nullptr);

    void addType();
    void deleteType(const QString &typeId);
    void moveType(const QString &typeId, int offset);
    void changeTypeColor(const QString &typeId, const QColor &color);
    void addInstance(const QString &typeId);
    void deleteInstance(const QString &instanceId);
    void assignType(const QVector<EntityID> &entityIds, const QString &typeId);
    void assignInstance(const QVector<EntityID> &entityIds, const QString &instanceId);

private:
    GAnnotationModel *m_document;
    QUndoStack *m_undoStack;
};

} // namespace totcad
