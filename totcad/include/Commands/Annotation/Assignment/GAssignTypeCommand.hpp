#pragma once
#include "Commands/Annotation/GAnnotationCommand.hpp"
#include "Model/Entities/GEntity.hpp"
namespace totcad {
class GAssignTypeCommand final : public GAnnotationCommand {
public: GAssignTypeCommand(GAnnotationModel *document, QVector<EntityID> entityIds, QString typeId);
protected: void apply() override;
private: QVector<EntityID> m_entityIds; QString m_typeId;
};
} // namespace totcad
