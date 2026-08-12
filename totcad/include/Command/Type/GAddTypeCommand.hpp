#pragma once

#include "Command/GAnnotationCommand.hpp"

namespace totcad {

class GAddTypeCommand final : public GAnnotationCommand
{
public:
    explicit GAddTypeCommand(GAnnotationModel *document);
    QString createdId() const { return m_createdId; }
protected:
    void apply() override;
private:
    QString m_createdId;
};

} // namespace totcad
