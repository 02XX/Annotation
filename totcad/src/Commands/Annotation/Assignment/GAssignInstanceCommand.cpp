#include "Commands/Annotation/Assignment/GAssignInstanceCommand.hpp"
namespace totcad {
GAssignInstanceCommand::GAssignInstanceCommand(GAnnotationModel *document, QVector<EntityID> entityIds, QString instanceId)
    : GAnnotationCommand(document, QObject::tr("实例标注")), m_entityIds(std::move(entityIds)), m_instanceId(std::move(instanceId)) {}
void GAssignInstanceCommand::apply() { document()->assignInstance(m_entityIds, m_instanceId); }
} // namespace totcad
