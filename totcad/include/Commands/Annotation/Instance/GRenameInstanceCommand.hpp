#pragma once
#include "Commands/Annotation/GAnnotationCommand.hpp"
namespace totcad {
class GRenameInstanceCommand final : public GAnnotationCommand {
public: GRenameInstanceCommand(GAnnotationModel *document, QString instanceId, QString name);
protected: void apply() override;
private: QString m_instanceId; QString m_name;
};
} // namespace totcad
