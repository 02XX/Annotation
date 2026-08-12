#include "Command/Assignment/GAssignTypeCommand.hpp"
namespace totcad {
GAssignTypeCommand::GAssignTypeCommand(GAnnotationDocument *document, QStringList entityIds, QString typeId)
    : GAnnotationCommand(document, QObject::tr("类型标注")), m_entityIds(std::move(entityIds)), m_typeId(std::move(typeId)) {}
void GAssignTypeCommand::apply() { document()->assignType(m_entityIds, m_typeId); }
} // namespace totcad
