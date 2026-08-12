#pragma once

#include "Model/GAnnotationModel.hpp"

#include <QUndoCommand>

namespace totcad {

class GAnnotationCommand : public QUndoCommand
{
public:
    GAnnotationCommand(GAnnotationModel *document, const QString &text, QUndoCommand *parent = nullptr);
    void redo() final;
    void undo() final;

protected:
    virtual void apply() = 0;
    GAnnotationModel *document() const { return m_document; }

private:
    GAnnotationModel *m_document;
    GAnnotationSnapshot m_before;
    GAnnotationSnapshot m_after;
    bool m_firstRun{true};
};

} // namespace totcad
