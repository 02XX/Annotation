#include "Controller/GToolController.hpp"
#include "Tools/Annotation/GInstanceAnnotationTool.hpp"
#include "Tools/Annotation/GTypeAnnotationTool.hpp"
#include "Tools/Navigation/GPanTool.hpp"
#include "Tools/Navigation/GRealtimeZoomTool.hpp"
#include "Tools/Navigation/GWindowZoomTool.hpp"
#include "Tools/Selection/GSelectTool.hpp"
#include "View/CAD/GCADView.hpp"
namespace totcad {
GToolController::GToolController(GCADView *view, QObject *parent)
    : QObject(parent), m_view(view),
      m_selectTool(new GSelectTool(view, this)),
      m_typeAnnotationTool(new GTypeAnnotationTool(view, this)),
      m_instanceAnnotationTool(new GInstanceAnnotationTool(view, this)),
      m_panTool(new GPanTool(view, this)),
      m_realtimeZoomTool(new GRealtimeZoomTool(view, this)),
      m_windowZoomTool(new GWindowZoomTool(view, this))
{
    connect(m_typeAnnotationTool, &GTypeAnnotationTool::assignmentRequested,
            this, &GToolController::typeAssignmentRequested);
    connect(m_instanceAnnotationTool, &GInstanceAnnotationTool::assignmentRequested,
            this, &GToolController::instanceAssignmentRequested);
    const auto cancel = [this] { activate(Mode::Select); };
    connect(m_typeAnnotationTool, &GCADTool::canceled, this, cancel);
    connect(m_instanceAnnotationTool, &GCADTool::canceled, this, cancel);
    activate(Mode::Select);
}
void GToolController::activate(Mode modeValue)
{
    m_mode = modeValue;
    switch (modeValue) {
    case Mode::Select: m_view->setActiveTool(m_selectTool); break;
    case Mode::TypeAnnotation: m_view->setActiveTool(m_typeAnnotationTool); break;
    case Mode::InstanceAnnotation: m_view->setActiveTool(m_instanceAnnotationTool); break;
    case Mode::Pan: m_view->setActiveTool(m_panTool); break;
    case Mode::RealtimeZoom: m_view->setActiveTool(m_realtimeZoomTool); break;
    case Mode::WindowZoom: m_view->setActiveTool(m_windowZoomTool); break;
    }
    emit modeChanged(modeValue);
}
} // namespace totcad
