#pragma once
#include <QDockWidget>
class QAction;
class QTreeView;
namespace totcad
{
class GInstanceTreeModel;
class GInstanceDockWidget final : public QDockWidget
{
    Q_OBJECT
  public:
    explicit GInstanceDockWidget(QWidget *parent = nullptr);
    void setModel(GInstanceTreeModel *model);
    QString currentInstanceId() const;
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
};
} // namespace totcad
