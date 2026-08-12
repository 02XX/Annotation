#include "Command/Instance/GDeleteInstanceCommand.hpp"
namespace totcad {
GDeleteInstanceCommand::GDeleteInstanceCommand(GAnnotationDocument *document, QString instanceId)
    : GAnnotationCommand(document, QObject::tr("删除实例")), m_instanceId(std::move(instanceId)) {}
void GDeleteInstanceCommand::apply() { document()->removeInstance(m_instanceId); }
} // namespace totcad
