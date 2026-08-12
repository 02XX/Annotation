#pragma once

#include <QColor>
#include <QObject>
#include <QStringList>

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
    QString addInstance(const QString &typeId);
    void deleteInstance(const QString &instanceId);
    void assignType(const QStringList &entityIds, const QString &typeId);
    void assignInstance(const QStringList &entityIds, const QString &instanceId);

private:
    GAnnotationDocument *m_document;
    QUndoStack *m_undoStack;
};

} // namespace totcad
