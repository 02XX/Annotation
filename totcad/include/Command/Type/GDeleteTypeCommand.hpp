#pragma once
#include "Command/GAnnotationCommand.hpp"
namespace totcad {
class GDeleteTypeCommand final : public GAnnotationCommand {
public: GDeleteTypeCommand(GAnnotationDocument *document, QString typeId);
protected: void apply() override;
private: QString m_typeId;
};
} // namespace totcad
