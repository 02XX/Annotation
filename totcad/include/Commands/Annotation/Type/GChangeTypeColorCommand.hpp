#pragma once
#include "Commands/Annotation/GAnnotationCommand.hpp"
namespace totcad {
class GChangeTypeColorCommand final : public GAnnotationCommand {
public: GChangeTypeColorCommand(GAnnotationModel *document, QString typeId, QColor color);
protected: void apply() override;
private: QString m_typeId; QColor m_color;
};
} // namespace totcad
