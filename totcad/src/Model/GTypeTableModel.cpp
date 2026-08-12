#include "Model/GTypeTableModel.hpp"

#include "Commands/Annotation/Type/GRenameTypeCommand.hpp"
#include "Model/GAnnotationModel.hpp"

#include <QBrush>
#include <QUndoStack>

namespace totcad {

GTypeTableModel::GTypeTableModel(GAnnotationModel *document, QUndoStack *undoStack, QObject *parent)
    : QAbstractTableModel(parent), m_document(document), m_undoStack(undoStack)
{
    const auto refresh = [this] { beginResetModel(); endResetModel(); };
    connect(document, &GAnnotationModel::typesChanged, this, refresh);
    connect(document, &GAnnotationModel::assignmentsChanged, this, refresh);
}

int GTypeTableModel::rowCount(const QModelIndex &parent) const { return parent.isValid() ? 0 : m_document->types().size(); }
int GTypeTableModel::columnCount(const QModelIndex &parent) const { return parent.isValid() ? 0 : 3; }

QVariant GTypeTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_document->types().size())
        return {};
    const GAnnotationType &value = m_document->types().at(index.row());
    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        if (index.column() == 0)
            return value.name;
        if (index.column() == 1)
            return value.color.name(QColor::HexRgb).toUpper();
        if (index.column() == 2)
            return m_document->assignedTypeEntityCount(value.id);
    }
    if (role == Qt::DecorationRole && index.column() == 1)
        return value.color;
    if (role == Qt::TextAlignmentRole && index.column() == 2)
        return Qt::AlignCenter;
    return {};
}

QVariant GTypeTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation != Qt::Horizontal || role != Qt::DisplayRole)
        return {};
    static const QStringList headers{tr("类型名"), tr("颜色"), tr("数量")};
    return headers.value(section);
}

Qt::ItemFlags GTypeTableModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags result = QAbstractTableModel::flags(index);
    if (index.isValid() && index.column() == 0)
        result |= Qt::ItemIsEditable;
    return result;
}

bool GTypeTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role != Qt::EditRole || !index.isValid() || index.column() != 0)
        return false;
    if (value.toString().trimmed().isEmpty())
        return false;
    m_undoStack->push(new GRenameTypeCommand(m_document, typeId(index.row()), value.toString()));
    return true;
}

QString GTypeTableModel::typeId(int row) const
{
    return row >= 0 && row < m_document->types().size() ? m_document->types().at(row).id : QString{};
}

} // namespace totcad
