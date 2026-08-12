#pragma once
#include "Model/Entities/GEntity.hpp"
#include <QObject>
#include <QVector>
namespace totcad {
class GView;
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
    explicit GToolController(GView *view, QObject *parent = nullptr);
    void activate(Mode mode);
    Mode mode() const noexcept { return m_mode; }
signals:
    void modeChanged(Mode mode);
    void typeAssignmentRequested(const QVector<EntityID> &entityIds);
    void instanceAssignmentRequested(const QVector<EntityID> &entityIds);
private:
    GView *m_view;
    GSelectTool *m_selectTool;
    GTypeAnnotationTool *m_typeAnnotationTool;
    GInstanceAnnotationTool *m_instanceAnnotationTool;
    GPanTool *m_panTool;
    GRealtimeZoomTool *m_realtimeZoomTool;
    GWindowZoomTool *m_windowZoomTool;
    Mode m_mode{Mode::Select};
};
} // namespace totcad
