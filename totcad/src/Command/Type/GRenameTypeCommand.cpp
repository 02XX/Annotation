#include "Command/Type/GRenameTypeCommand.hpp"
namespace totcad {
GRenameTypeCommand::GRenameTypeCommand(GAnnotationDocument *document, QString typeId, QString name)
    : GAnnotationCommand(document, QObject::tr("重命名类型")), m_typeId(std::move(typeId)), m_name(std::move(name)) {}
void GRenameTypeCommand::apply() { document()->setTypeName(m_typeId, m_name); }
} // namespace totcad
