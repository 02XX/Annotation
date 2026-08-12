#pragma once
#include "Command/GAnnotationCommand.hpp"
namespace totcad {
class GDeleteTypeCommand final : public GAnnotationCommand {
public: GDeleteTypeCommand(GAnnotationModel *document, QString typeId);
protected: void apply() override;
private: QString m_typeId;
};
} // namespace totcad
