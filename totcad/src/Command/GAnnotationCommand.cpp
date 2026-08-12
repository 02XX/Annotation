#include "Command/GAnnotationCommand.hpp"

namespace totcad {

GAnnotationCommand::GAnnotationCommand(GAnnotationDocument *document,
                                     const QString &text,
                                     QUndoCommand *parent)
    : QUndoCommand(text, parent), m_document(document), m_before(document->snapshot())
{
}

void GAnnotationCommand::redo()
{
    if (m_firstRun) {
        apply();
        m_after = m_document->snapshot();
        m_firstRun = false;
    } else {
        m_document->restore(m_after);
    }
}

void GAnnotationCommand::undo()
{
    m_document->restore(m_before);
}

} // namespace totcad
