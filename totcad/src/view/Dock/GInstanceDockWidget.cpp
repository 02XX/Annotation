#include "View/Dock/GInstanceDockWidget.hpp"
#include "Model/GInstanceTreeModel.hpp"
#include <QAction>
#include <QToolBar>
#include <QTreeView>
#include <QVBoxLayout>
namespace totcad {
GInstanceDockWidget::GInstanceDockWidget(QWidget *parent) : QDockWidget(tr("实例"), parent)
{
    setObjectName(QStringLiteral("InstanceDock"));
    auto *contents = new QWidget(this);
    auto *layout = new QVBoxLayout(contents);
    layout->setContentsMargins(0, 0, 0, 0);
    auto *toolbar = new QToolBar(contents);
    m_addAction = toolbar->addAction(tr("新增"));
    m_deleteAction = toolbar->addAction(tr("删除"));
    m_tree = new QTreeView(contents);
    m_tree->setAlternatingRowColors(true);
    m_tree->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::EditKeyPressed);
    layout->addWidget(toolbar);
    layout->addWidget(m_tree);
    setWidget(contents);
    connect(m_addAction, &QAction::triggered, this, &GInstanceDockWidget::addRequested);
    connect(m_deleteAction, &QAction::triggered, this, [this] { emit deleteRequested(currentInstanceId()); });
    updateActions();
}
void GInstanceDockWidget::setModel(GInstanceTreeModel *model)
{
    m_model = model;
    m_tree->setModel(model);
    connect(m_tree->selectionModel(), &QItemSelectionModel::currentChanged, this,
            [this](const QModelIndex &, const QModelIndex &) {
                updateActions();
                const QString id = currentInstanceId();
                if (!id.isEmpty()) emit instanceActivated(id);
            });
    connect(model, &QAbstractItemModel::modelReset, this, [this] {
        m_tree->expandAll();
        updateActions();
    });
    m_tree->expandAll();
    updateActions();
}
QString GInstanceDockWidget::currentInstanceId() const
{
    return m_model ? m_model->instanceId(m_tree->currentIndex()) : QString{};
}
void GInstanceDockWidget::updateActions()
{
    m_addAction->setEnabled(m_model && !m_model->currentType().isEmpty());
    m_deleteAction->setEnabled(!currentInstanceId().isEmpty());
}
} // namespace totcad
