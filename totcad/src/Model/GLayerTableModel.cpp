#include "Model/GLayerTableModel.hpp"

#include "Model/CAD/GCADDocument.hpp"

namespace totcad {

GLayerTableModel::GLayerTableModel(QObject *parent) : QAbstractTableModel(parent) {}

void GLayerTableModel::setDocument(GCADDocument *document)
{
    beginResetModel();
    m_document = document;
    m_layerNames = document ? document->layers().keys() : QStringList{};
    m_layerNames.sort(Qt::CaseInsensitive);
    endResetModel();
}

int GLayerTableModel::rowCount(const QModelIndex &parent) const { return parent.isValid() ? 0 : m_layerNames.size(); }
int GLayerTableModel::columnCount(const QModelIndex &parent) const { return parent.isValid() ? 0 : 5; }

QVariant GLayerTableModel::data(const QModelIndex &index, int role) const
{
    if (!m_document || !index.isValid() || index.row() >= m_layerNames.size())
        return {};
    const GCADLayer &layer = m_document->layers().value(m_layerNames.at(index.row()));
    if (role == Qt::DisplayRole) {
        switch (index.column()) {
        case 0: return layer.name;
        case 1: return layer.color.name(QColor::HexRgb).toUpper();
        case 2: return layer.lineWidth <= 0.0 ? tr("默认") : QString::number(layer.lineWidth);
        case 3: return layer.lineTypeName;
        default: return {};
        }
    }
    if (role == Qt::DecorationRole && index.column() == 1)
        return layer.color;
    if (role == Qt::CheckStateRole && index.column() == 4)
        return layer.visible ? Qt::Checked : Qt::Unchecked;
    return {};
}

QVariant GLayerTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation != Qt::Horizontal || role != Qt::DisplayRole)
        return {};
    static const QStringList headers{tr("名称"), tr("颜色"), tr("线宽"), tr("线型"), tr("可见性")};
    return headers.value(section);
}

Qt::ItemFlags GLayerTableModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags result = QAbstractTableModel::flags(index);
    if (index.isValid() && index.column() == 4)
        result |= Qt::ItemIsUserCheckable;
    return result;
}

bool GLayerTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!m_document || role != Qt::CheckStateRole || index.column() != 4)
        return false;
    GCADLayer &layer = m_document->layers()[m_layerNames.at(index.row())];
    layer.visible = value.toInt() == Qt::Checked;
    emit dataChanged(index, index, {Qt::CheckStateRole});
    emit m_document->layerVisibilityChanged(layer.name, layer.visible);
    return true;
}

} // namespace totcad
