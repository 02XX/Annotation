#pragma once

#include "Model/Entities/GEntity.hpp"

#include <QColor>
#include <QObject>
#include <QVector>

class QUndoStack;

namespace totcad {

class GAnnotationDocument;

class GAnnotationController final : public QObject
{
    Q_OBJECT
public:
    GAnnotationController(GAnnotationDocument *document, QUndoStack *undoStack, QObject *parent = nullptr);

    void addType();
    void deleteType(const QString &typeId);
    void moveType(const QString &typeId, int offset);
    void changeTypeColor(const QString &typeId, const QColor &color);
    void addInstance(const QString &typeId);
    void deleteInstance(const QString &instanceId);
    void assignType(const QVector<EntityID> &entityIds, const QString &typeId);
    void assignInstance(const QVector<EntityID> &entityIds, const QString &instanceId);

private:
    GAnnotationDocument *m_document;
    QUndoStack *m_undoStack;
};

} // namespace totcad
