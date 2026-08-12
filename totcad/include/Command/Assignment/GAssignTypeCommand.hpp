#pragma once
#include "Command/GAnnotationCommand.hpp"
namespace totcad {
class GAssignTypeCommand final : public GAnnotationCommand {
public: GAssignTypeCommand(GAnnotationDocument *document, QStringList entityIds, QString typeId);
protected: void apply() override;
private: QStringList m_entityIds; QString m_typeId;
};
} // namespace totcad
