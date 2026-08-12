#pragma once
#include "Commands/Annotation/GAnnotationCommand.hpp"
namespace totcad {
class GDeleteInstanceCommand final : public GAnnotationCommand {
public: GDeleteInstanceCommand(GAnnotationModel *document, QString instanceId);
protected: void apply() override;
private: QString m_instanceId;
};
} // namespace totcad
