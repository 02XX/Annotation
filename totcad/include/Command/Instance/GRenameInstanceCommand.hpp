#pragma once
#include "Command/GAnnotationCommand.hpp"
namespace totcad {
class GRenameInstanceCommand final : public GAnnotationCommand {
public: GRenameInstanceCommand(GAnnotationDocument *document, QString instanceId, QString name);
protected: void apply() override;
private: QString m_instanceId; QString m_name;
};
} // namespace totcad
