#pragma once
#include <QDockWidget>
class QAction;
class QModelIndex;
class QTableView;
namespace totcad
{
class GTypeTableModel;
class GTypeDockWidget final : public QDockWidget
{
    Q_OBJECT
  public:
    explicit GTypeDockWidget(QWidget *parent = nullptr);
    void setModel(GTypeTableModel *model);
    QString currentTypeId() const;
    int currentRow() const;
    void selectRow(int row);
  signals:
    void addRequested();
    void deleteRequested(const QString &typeId);
    void moveRequested(const QString &typeId, int offset);
    void colorRequested(const QString &typeId);
    void currentTypeChanged(const QString &typeId);

  private:
    void updateActions();
    GTypeTableModel *m_model{nullptr};
    QTableView *m_table{nullptr};
    QAction *m_deleteAction{nullptr};
    QAction *m_upAction{nullptr};
    QAction *m_downAction{nullptr};
};
} // namespace totcad
