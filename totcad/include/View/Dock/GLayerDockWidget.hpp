#pragma once
#include <QDockWidget>
class QTableView;
namespace totcad {
class GLayerTableModel;
class GLayerDockWidget final : public QDockWidget {
    Q_OBJECT
public:
    explicit GLayerDockWidget(QWidget *parent = nullptr);
    void setModel(GLayerTableModel *model);
private:
    QTableView *m_table{nullptr};
};
} // namespace totcad
