#pragma once

#include <QObject>
#include <QCursor>

class QKeyEvent;
class QMouseEvent;

namespace totcad {

class GGraphicsView;

class GCanvasTool : public QObject
{
    Q_OBJECT
public:
    explicit GCanvasTool(GGraphicsView *view, QObject *parent = nullptr);
    virtual ~GCanvasTool() = default;

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
    GGraphicsView *view() const noexcept { return m_view; }

private:
    GGraphicsView *m_view;
};

} // namespace totcad
