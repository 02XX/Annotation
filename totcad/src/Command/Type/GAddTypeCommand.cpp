#include "Command/Type/GAddTypeCommand.hpp"

namespace totcad {
GAddTypeCommand::GAddTypeCommand(GAnnotationDocument *document) : GAnnotationCommand(document, QObject::tr("新增类型")) {}
void GAddTypeCommand::apply() { m_createdId = document()->addType(); }
} // namespace totcad
