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
                if (m_modelResetting)
                    return;
                m_currentInstanceId = m_model ? m_model->instanceId(m_tree->currentIndex()) : QString{};
                updateActions();
                if (!m_currentInstanceId.isEmpty()) emit instanceActivated(m_currentInstanceId);
            });
    connect(model, &QAbstractItemModel::modelAboutToBeReset, this, [this] {
        m_modelResetting = true;
    });
    connect(model, &QAbstractItemModel::modelReset, this, [this] {
        m_modelResetting = false;
        m_tree->expandAll();
        const QModelIndex target = m_model->indexForInstanceId(m_currentInstanceId);
        if (target.isValid()) {
            selectInstance(m_currentInstanceId);
        } else {
            m_currentInstanceId.clear();
            updateActions();
        }
    });
    m_tree->expandAll();
    updateActions();
}
QString GInstanceDockWidget::currentInstanceId() const
{
    return m_currentInstanceId;
}
void GInstanceDockWidget::selectInstance(const QString &instanceId)
{
    if (!m_model)
        return;
    const QModelIndex target = m_model->indexForInstanceId(instanceId);
    if (!target.isValid())
        return;
    m_tree->expand(target.parent());
    m_tree->setCurrentIndex(target);
    m_tree->scrollTo(target);
}
void GInstanceDockWidget::updateActions()
{
    m_addAction->setEnabled(m_model && !m_model->currentType().isEmpty());
    m_deleteAction->setEnabled(!currentInstanceId().isEmpty());
}
} // namespace totcad
