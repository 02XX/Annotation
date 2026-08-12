#include "View/Dock/GLayerDockWidget.hpp"
#include "Model/GLayerTableModel.hpp"
#include <QHeaderView>
#include <QTableView>
namespace totcad {
GLayerDockWidget::GLayerDockWidget(QWidget *parent) : QDockWidget(tr("图层"), parent)
{
    setObjectName(QStringLiteral("LayerDock"));
    m_table = new QTableView(this);
    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_table->setAlternatingRowColors(true);
    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    setWidget(m_table);
}
void GLayerDockWidget::setModel(GLayerTableModel *model)
{
    m_table->setModel(model);
    m_table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    for (int i = 1; i < 5; ++i)
        m_table->horizontalHeader()->setSectionResizeMode(i, QHeaderView::ResizeToContents);
}
} // namespace totcad
