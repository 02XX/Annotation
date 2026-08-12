#pragma once
#include "Command/GAnnotationCommand.hpp"
namespace totcad {
class GDeleteInstanceCommand final : public GAnnotationCommand {
public: GDeleteInstanceCommand(GAnnotationDocument *document, QString instanceId);
protected: void apply() override;
private: QString m_instanceId;
};
} // namespace totcad
