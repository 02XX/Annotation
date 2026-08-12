#pragma once
#include "Command/GAnnotationCommand.hpp"
namespace totcad {
class GAssignInstanceCommand final : public GAnnotationCommand {
public: GAssignInstanceCommand(GAnnotationDocument *document, QStringList entityIds, QString instanceId);
protected: void apply() override;
private: QStringList m_entityIds; QString m_instanceId;
};
} // namespace totcad
