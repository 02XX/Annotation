#pragma once

#include <QObject>
#include <QCursor>

class QKeyEvent;
class QMouseEvent;

namespace totcad {

class GView;

class GTool : public QObject
{
    Q_OBJECT
public:
    explicit GTool(GView *view, QObject *parent = nullptr);
    virtual ~GTool() = default;

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
    GView *view() const noexcept { return m_view; }

private:
    GView *m_view;
};

} // namespace totcad
