#include "Command/Instance/GRenameInstanceCommand.hpp"
namespace totcad {
GRenameInstanceCommand::GRenameInstanceCommand(GAnnotationModel *document, QString instanceId, QString name)
    : GAnnotationCommand(document, QObject::tr("重命名实例")), m_instanceId(std::move(instanceId)), m_name(std::move(name)) {}
void GRenameInstanceCommand::apply() { document()->setInstanceName(m_instanceId, m_name); }
} // namespace totcad
