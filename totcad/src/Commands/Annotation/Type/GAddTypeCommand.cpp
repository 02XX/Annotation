#include "Commands/Annotation/Type/GAddTypeCommand.hpp"

namespace totcad {
GAddTypeCommand::GAddTypeCommand(GAnnotationModel *document) : GAnnotationCommand(document, QObject::tr("新增类型")) {}
void GAddTypeCommand::apply() { m_createdId = document()->addType(); }
} // namespace totcad
