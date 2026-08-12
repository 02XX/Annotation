#pragma once
#include "Command/GAnnotationCommand.hpp"
namespace totcad {
class GMoveTypeCommand final : public GAnnotationCommand {
public: GMoveTypeCommand(GAnnotationModel *document, QString typeId, int offset);
protected: void apply() override;
private: QString m_typeId; int m_offset;
};
} // namespace totcad
