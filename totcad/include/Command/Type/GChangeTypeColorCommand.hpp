#pragma once
#include "Command/GAnnotationCommand.hpp"
namespace totcad {
class GChangeTypeColorCommand final : public GAnnotationCommand {
public: GChangeTypeColorCommand(GAnnotationDocument *document, QString typeId, QColor color);
protected: void apply() override;
private: QString m_typeId; QColor m_color;
};
} // namespace totcad
