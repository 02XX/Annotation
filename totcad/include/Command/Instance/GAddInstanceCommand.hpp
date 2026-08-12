#pragma once
#include "Command/GAnnotationCommand.hpp"
namespace totcad {
class GAddInstanceCommand final : public GAnnotationCommand {
public: GAddInstanceCommand(GAnnotationModel *document, QString typeId); QString createdId() const { return m_createdId; }
protected: void apply() override;
private: QString m_typeId; QString m_createdId;
};
} // namespace totcad
