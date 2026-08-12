#include "Model/GInstanceTreeModel.hpp"

#include "Commands/Annotation/Instance/GRenameInstanceCommand.hpp"
#include "Model/GAnnotationModel.hpp"

#include <QUndoStack>

namespace totcad {

GInstanceTreeModel::GInstanceTreeModel(GAnnotationModel *document, QUndoStack *undoStack, QObject *parent)
    : QAbstractItemModel(parent), m_document(document), m_undoStack(undoStack)
{
    const auto refresh = [this] { beginResetModel(); endResetModel(); };
    connect(document, &GAnnotationModel::instancesChanged, this, refresh);
    connect(document, &GAnnotationModel::typesChanged, this, refresh);
}

QModelIndex GInstanceTreeModel::index(int row, int column, const QModelIndex &parentIndex) const
{
    if (column != 0 || row < 0)
        return {};
    if (!parentIndex.isValid() && row == 0 && !m_typeId.isEmpty())
        return createIndex(row, column, quintptr(1));
    if (parentIndex.isValid() && parentIndex.internalId() == 1 && row < m_document->instancesForType(m_typeId).size())
        return createIndex(row, column, quintptr(2));
    return {};
}

QModelIndex GInstanceTreeModel::parent(const QModelIndex &child) const
{
    return child.isValid() && child.internalId() == 2 ? createIndex(0, 0, quintptr(1)) : QModelIndex{};
}

int GInstanceTreeModel::rowCount(const QModelIndex &parentIndex) const
{
    if (m_typeId.isEmpty())
        return 0;
    if (!parentIndex.isValid())
        return 1;
    return parentIndex.internalId() == 1 ? m_document->instancesForType(m_typeId).size() : 0;
}

int GInstanceTreeModel::columnCount(const QModelIndex &) const { return 1; }

QVariant GInstanceTreeModel::data(const QModelIndex &indexValue, int role) const
{
    if (!indexValue.isValid() || (role != Qt::DisplayRole && role != Qt::EditRole))
        return {};
    if (indexValue.internalId() == 1) {
        const GAnnotationType *value = m_document->type(m_typeId);
        const int count = m_document->instancesForType(m_typeId).size();
        return value ? tr("%1 (%2)").arg(value->name).arg(count) : QVariant{};
    }
    const QVector<GAnnotationInstance> values = m_document->instancesForType(m_typeId);
    return indexValue.row() < values.size() ? values.at(indexValue.row()).name : QVariant{};
}

QVariant GInstanceTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    return section == 0 && orientation == Qt::Horizontal && role == Qt::DisplayRole ? tr("实例") : QVariant{};
}

Qt::ItemFlags GInstanceTreeModel::flags(const QModelIndex &indexValue) const
{
    Qt::ItemFlags result = QAbstractItemModel::flags(indexValue);
    if (indexValue.isValid() && indexValue.internalId() == 2)
        result |= Qt::ItemIsEditable;
    return result;
}

bool GInstanceTreeModel::setData(const QModelIndex &indexValue, const QVariant &value, int role)
{
    if (role != Qt::EditRole || value.toString().trimmed().isEmpty())
        return false;
    const QString id = instanceId(indexValue);
    if (id.isEmpty())
        return false;
    m_undoStack->push(new GRenameInstanceCommand(m_document, id, value.toString()));
    return true;
}

void GInstanceTreeModel::setCurrentType(const QString &typeId)
{
    if (m_typeId == typeId)
        return;
    beginResetModel();
    m_typeId = typeId;
    endResetModel();
}

QString GInstanceTreeModel::instanceId(const QModelIndex &indexValue) const
{
    if (!indexValue.isValid() || indexValue.internalId() != 2)
        return {};
    const auto values = m_document->instancesForType(m_typeId);
    return indexValue.row() < values.size() ? values.at(indexValue.row()).id : QString{};
}

} // namespace totcad
