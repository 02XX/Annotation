#pragma once
#include <QDockWidget>
#include <QString>
class QAction;
class QTreeView;
namespace totcad {
class GInstanceTreeModel;
class GInstanceDockWidget final : public QDockWidget {
    Q_OBJECT
public:
    explicit GInstanceDockWidget(QWidget *parent = nullptr);
    void setModel(GInstanceTreeModel *model);
    QString currentInstanceId() const;
    void selectInstance(const QString &instanceId);
signals:
    void addRequested();
    void deleteRequested(const QString &instanceId);
    void instanceActivated(const QString &instanceId);
private:
    void updateActions();
    GInstanceTreeModel *m_model{nullptr};
    QTreeView *m_tree{nullptr};
    QAction *m_addAction{nullptr};
    QAction *m_deleteAction{nullptr};
    QString m_currentInstanceId;
    bool m_modelResetting{false};
};
} // namespace totcad
