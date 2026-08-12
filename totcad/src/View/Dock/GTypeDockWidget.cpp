#include "View/Dock/GTypeDockWidget.hpp"
#include "Model/GTypeTableModel.hpp"
#include <QAction>
#include <QHeaderView>
#include <QTableView>
#include <QToolBar>
#include <QVBoxLayout>
namespace totcad {
GTypeDockWidget::GTypeDockWidget(QWidget *parent) : QDockWidget(tr("类型"), parent)
{
    setObjectName(QStringLiteral("TypeDock"));
    auto *contents = new QWidget(this);
    auto *layout = new QVBoxLayout(contents);
    layout->setContentsMargins(0, 0, 0, 0);
    auto *toolbar = new QToolBar(contents);
    toolbar->setIconSize(QSize(16, 16));
    QAction *addAction = toolbar->addAction(tr("新增"));
    m_deleteAction = toolbar->addAction(tr("删除"));
    m_upAction = toolbar->addAction(tr("上移"));
    m_downAction = toolbar->addAction(tr("下移"));
    m_table = new QTableView(contents);
    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_table->setSelectionMode(QAbstractItemView::SingleSelection);
    m_table->setAlternatingRowColors(true);
    m_table->horizontalHeader()->setStretchLastSection(true);
    layout->addWidget(toolbar);
    layout->addWidget(m_table);
    setWidget(contents);
    connect(addAction, &QAction::triggered, this, &GTypeDockWidget::addRequested);
    connect(m_deleteAction, &QAction::triggered, this, [this] { emit deleteRequested(currentTypeId()); });
    connect(m_upAction, &QAction::triggered, this, [this] { emit moveRequested(currentTypeId(), -1); });
    connect(m_downAction, &QAction::triggered, this, [this] { emit moveRequested(currentTypeId(), 1); });
    connect(m_table, &QTableView::doubleClicked, this, [this](const QModelIndex &index) {
        if (index.column() == 1) emit colorRequested(currentTypeId());
    });
    updateActions();
}
void GTypeDockWidget::setModel(GTypeTableModel *model)
{
    m_model = model;
    m_table->setModel(model);
    m_table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    m_table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    m_table->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    connect(m_table->selectionModel(), &QItemSelectionModel::currentRowChanged, this,
            [this](const QModelIndex &, const QModelIndex &) {
                if (m_modelResetting)
                    return;
                m_currentTypeId = m_model ? m_model->typeId(currentRow()) : QString{};
                updateActions();
                emit currentTypeChanged(m_currentTypeId);
            });
    connect(model, &QAbstractItemModel::modelAboutToBeReset, this, [this] {
        m_modelResetting = true;
        m_rowBeforeReset = currentRow();
    });
    connect(model, &QAbstractItemModel::modelReset, this, [this] {
        m_modelResetting = false;
        int row = m_model->rowForTypeId(m_currentTypeId);
        if (row < 0 && m_model->rowCount() > 0)
            row = qBound(0, m_rowBeforeReset, m_model->rowCount() - 1);
        if (row >= 0) {
            selectRow(row);
        } else {
            m_currentTypeId.clear();
            updateActions();
            emit currentTypeChanged({});
        }
    });
    updateActions();
}
QString GTypeDockWidget::currentTypeId() const { return m_currentTypeId; }
int GTypeDockWidget::currentRow() const { return m_table->currentIndex().row(); }
void GTypeDockWidget::selectRow(int row)
{
    if (!m_model || row < 0 || row >= m_model->rowCount()) return;
    m_table->setCurrentIndex(m_model->index(row, 0));
}
void GTypeDockWidget::updateActions()
{
    const int row = currentRow();
    const int count = m_model ? m_model->rowCount() : 0;
    m_deleteAction->setEnabled(row >= 0);
    m_upAction->setEnabled(row > 0);
    m_downAction->setEnabled(row >= 0 && row + 1 < count);
}
} // namespace totcad
