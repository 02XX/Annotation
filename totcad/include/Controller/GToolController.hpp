#pragma once
#include <QObject>
namespace totcad {
class GCADView;
class GSelectTool;
class GTypeAnnotationTool;
class GInstanceAnnotationTool;
class GPanTool;
class GRealtimeZoomTool;
class GWindowZoomTool;

class GToolController final : public QObject {
    Q_OBJECT
public:
    enum class Mode { Select, TypeAnnotation, InstanceAnnotation, Pan, RealtimeZoom, WindowZoom };
    explicit GToolController(GCADView *view, QObject *parent = nullptr);
    void activate(Mode mode);
    Mode mode() const noexcept { return m_mode; }
signals:
    void modeChanged(Mode mode);
    void typeAssignmentRequested(const QStringList &entityIds);
    void instanceAssignmentRequested(const QStringList &entityIds);
private:
    GCADView *m_view;
    GSelectTool *m_selectTool;
    GTypeAnnotationTool *m_typeAnnotationTool;
    GInstanceAnnotationTool *m_instanceAnnotationTool;
    GPanTool *m_panTool;
    GRealtimeZoomTool *m_realtimeZoomTool;
    GWindowZoomTool *m_windowZoomTool;
    Mode m_mode{Mode::Select};
};
} // namespace totcad
