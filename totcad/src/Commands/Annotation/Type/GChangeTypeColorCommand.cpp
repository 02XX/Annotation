#include "Commands/Annotation/Type/GChangeTypeColorCommand.hpp"
namespace totcad {
GChangeTypeColorCommand::GChangeTypeColorCommand(GAnnotationModel *document, QString typeId, QColor color)
    : GAnnotationCommand(document, QObject::tr("修改类型颜色")), m_typeId(std::move(typeId)), m_color(std::move(color)) {}
void GChangeTypeColorCommand::apply() { document()->setTypeColor(m_typeId, m_color); }
} // namespace totcad
