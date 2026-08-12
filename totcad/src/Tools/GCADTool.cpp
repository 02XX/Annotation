#include "Tools/GCADTool.hpp"

#include "View/CAD/GCADView.hpp"

namespace totcad {

GCADTool::GCADTool(GCADView *viewValue, QObject *parent) : QObject(parent), m_view(viewValue) {}
void GCADTool::activate() { m_view->setCursor(cursor()); }
void GCADTool::deactivate() { m_view->setCursor(Qt::CrossCursor); }
bool GCADTool::mousePress(QMouseEvent *) { return false; }
bool GCADTool::mouseMove(QMouseEvent *) { return false; }
bool GCADTool::mouseRelease(QMouseEvent *) { return false; }
bool GCADTool::keyPress(QKeyEvent *) { return false; }

} // namespace totcad
