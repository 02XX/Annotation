#pragma once

#include "Model/Annotation/GAnnotationDocument.hpp"

#include <QUndoCommand>

namespace totcad {

class GAnnotationCommand : public QUndoCommand
{
public:
    GAnnotationCommand(GAnnotationDocument *document, const QString &text, QUndoCommand *parent = nullptr);
    void redo() final;
    void undo() final;

protected:
    virtual void apply() = 0;
    GAnnotationDocument *document() const { return m_document; }

private:
    GAnnotationDocument *m_document;
    GAnnotationSnapshot m_before;
    GAnnotationSnapshot m_after;
    bool m_firstRun{true};
};

} // namespace totcad
