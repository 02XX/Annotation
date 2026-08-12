#pragma once
#include "Commands/Annotation/GAnnotationCommand.hpp"
#include "Model/Entities/GEntity.hpp"
namespace totcad {
class GAssignInstanceCommand final : public GAnnotationCommand {
public: GAssignInstanceCommand(GAnnotationModel *document, QVector<EntityID> entityIds, QString instanceId);
protected: void apply() override;
private: QVector<EntityID> m_entityIds; QString m_instanceId;
};
} // namespace totcad
