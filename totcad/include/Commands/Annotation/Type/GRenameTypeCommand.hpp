#pragma once
#include "Commands/Annotation/GAnnotationCommand.hpp"
namespace totcad {
class GRenameTypeCommand final : public GAnnotationCommand {
public: GRenameTypeCommand(GAnnotationModel *document, QString typeId, QString name);
protected: void apply() override;
private: QString m_typeId; QString m_name;
};
} // namespace totcad
