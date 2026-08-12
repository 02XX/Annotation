#include "Command/Instance/GAddInstanceCommand.hpp"
namespace totcad {
GAddInstanceCommand::GAddInstanceCommand(GAnnotationModel *document, QString typeId)
    : GAnnotationCommand(document, QObject::tr("新增实例")), m_typeId(std::move(typeId)) {}
void GAddInstanceCommand::apply() { m_createdId = document()->addInstance(m_typeId); }
} // namespace totcad
