#include "Tools/GTool.hpp"

#include "Graphics/GView.hpp"

namespace totcad {

GTool::GTool(GView *viewValue, QObject *parent) : QObject(parent), m_view(viewValue) {}
void GTool::activate() { m_view->setCursor(cursor()); }
void GTool::deactivate() { m_view->setCursor(Qt::CrossCursor); }
bool GTool::mousePress(QMouseEvent *) { return false; }
bool GTool::mouseMove(QMouseEvent *) { return false; }
bool GTool::mouseRelease(QMouseEvent *) { return false; }
bool GTool::keyPress(QKeyEvent *) { return false; }

} // namespace totcad
