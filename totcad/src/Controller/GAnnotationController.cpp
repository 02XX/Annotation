#include "Controller/GAnnotationController.hpp"

#include "Command/Assignment/GAssignInstanceCommand.hpp"
#include "Command/Assignment/GAssignTypeCommand.hpp"
#include "Command/Instance/GAddInstanceCommand.hpp"
#include "Command/Instance/GDeleteInstanceCommand.hpp"
#include "Command/Type/GAddTypeCommand.hpp"
#include "Command/Type/GChangeTypeColorCommand.hpp"
#include "Command/Type/GDeleteTypeCommand.hpp"
#include "Command/Type/GMoveTypeCommand.hpp"

#include <QUndoStack>

namespace totcad
{

GAnnotationController::GAnnotationController(GAnnotationDocument *document, QUndoStack *undoStack, QObject *parent)
    : QObject(parent), m_document(document), m_undoStack(undoStack)
{
}

void GAnnotationController::addType()
{
    m_undoStack->push(new GAddTypeCommand(m_document));
}
void GAnnotationController::deleteType(const QString &id)
{
    if (!id.isEmpty())
        m_undoStack->push(new GDeleteTypeCommand(m_document, id));
}
void GAnnotationController::moveType(const QString &id, int offset)
{
    if (!id.isEmpty())
        m_undoStack->push(new GMoveTypeCommand(m_document, id, offset));
}
void GAnnotationController::changeTypeColor(const QString &id, const QColor &color)
{
    if (!id.isEmpty() && color.isValid())
        m_undoStack->push(new GChangeTypeColorCommand(m_document, id, color));
}
void GAnnotationController::addInstance(const QString &typeId)
{
    if (!typeId.isEmpty())
        m_undoStack->push(new GAddInstanceCommand(m_document, typeId));
}
void GAnnotationController::deleteInstance(const QString &id)
{
    if (!id.isEmpty())
        m_undoStack->push(new GDeleteInstanceCommand(m_document, id));
}
void GAnnotationController::assignType(const QVector<EntityID> &ids, const QString &typeId)
{
    if (!ids.isEmpty() && !typeId.isEmpty())
        m_undoStack->push(new GAssignTypeCommand(m_document, ids, typeId));
}
void GAnnotationController::assignInstance(const QVector<EntityID> &ids, const QString &instanceId)
{
    if (!ids.isEmpty() && !instanceId.isEmpty())
        m_undoStack->push(new GAssignInstanceCommand(m_document, ids, instanceId));
}

} // namespace totcad
