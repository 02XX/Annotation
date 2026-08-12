#include "Command/Type/GDeleteTypeCommand.hpp"
namespace totcad {
GDeleteTypeCommand::GDeleteTypeCommand(GAnnotationModel *document, QString typeId)
    : GAnnotationCommand(document, QObject::tr("删除类型")), m_typeId(std::move(typeId)) {}
void GDeleteTypeCommand::apply() { document()->removeType(m_typeId); }
} // namespace totcad
