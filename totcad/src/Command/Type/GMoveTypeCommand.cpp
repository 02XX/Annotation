#include "Command/Type/GMoveTypeCommand.hpp"
namespace totcad {
GMoveTypeCommand::GMoveTypeCommand(GAnnotationDocument *document, QString typeId, int offset)
    : GAnnotationCommand(document, offset < 0 ? QObject::tr("上移类型") : QObject::tr("下移类型")),
      m_typeId(std::move(typeId)), m_offset(offset) {}
void GMoveTypeCommand::apply() { document()->moveType(m_typeId, m_offset); }
} // namespace totcad
