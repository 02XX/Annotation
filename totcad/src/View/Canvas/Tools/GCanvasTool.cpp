#include "View/Canvas/Tools/GCanvasTool.hpp"

#include "View/Canvas/GGraphicsView.hpp"

namespace totcad {

GCanvasTool::GCanvasTool(GGraphicsView *viewValue, QObject *parent) : QObject(parent), m_view(viewValue) {}
void GCanvasTool::activate() { m_view->setCursor(cursor()); }
void GCanvasTool::deactivate() { m_view->setCursor(Qt::CrossCursor); }
bool GCanvasTool::mousePress(QMouseEvent *) { return false; }
bool GCanvasTool::mouseMove(QMouseEvent *) { return false; }
bool GCanvasTool::mouseRelease(QMouseEvent *) { return false; }
bool GCanvasTool::keyPress(QKeyEvent *) { return false; }

} // namespace totcad
