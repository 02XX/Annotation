#pragma once

#include <QObject>
#include <QCursor>

class QKeyEvent;
class QMouseEvent;

namespace totcad {

class GCADView;

class GCADTool : public QObject
{
    Q_OBJECT
public:
    explicit GCADTool(GCADView *view, QObject *parent = nullptr);
    virtual ~GCADTool() = default;

    virtual void activate();
    virtual void deactivate();
    virtual bool mousePress(QMouseEvent *event);
    virtual bool mouseMove(QMouseEvent *event);
    virtual bool mouseRelease(QMouseEvent *event);
    virtual bool keyPress(QKeyEvent *event);
    virtual QCursor cursor() const { return Qt::CrossCursor; }

signals:
    void canceled();

protected:
    GCADView *view() const noexcept { return m_view; }

private:
    GCADView *m_view;
};

} // namespace totcad
