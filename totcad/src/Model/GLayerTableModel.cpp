#include "Model/GLayerTableModel.hpp"

#include "Model/GDXFModel.hpp"

#include <algorithm>
#include <cmath>

namespace {

QColor aciColor(int colorIndex)
{
    switch (std::abs(colorIndex)) {
    case 1: return Qt::red;
    case 2: return Qt::yellow;
    case 3: return Qt::green;
    case 4: return Qt::cyan;
    case 5: return Qt::blue;
    case 6: return Qt::magenta;
    case 7: return Qt::white;
    default:
        return QColor::fromHsv(((std::abs(colorIndex) - 10) * 17) % 360, 180, 230);
    }
}

} // namespace

namespace totcad {

GLayerTableModel::GLayerTableModel(QObject *parent) : QAbstractTableModel(parent) {}

void GLayerTableModel::setDrawing(GDXFModel *drawing)
{
    beginResetModel();
    m_drawing = drawing;
    m_layerNames.clear();
    if (drawing) {
        m_layerNames.reserve(drawing->layers().size());
        for (const auto &entry : drawing->layers())
            m_layerNames.push_back(entry.first);
        std::sort(m_layerNames.begin(), m_layerNames.end());
    }
    endResetModel();
}

int GLayerTableModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : static_cast<int>(m_layerNames.size());
}
int GLayerTableModel::columnCount(const QModelIndex &parent) const { return parent.isValid() ? 0 : 5; }

QVariant GLayerTableModel::data(const QModelIndex &index, int role) const
{
    if (!m_drawing || !index.isValid()
        || static_cast<std::size_t>(index.row()) >= m_layerNames.size())
        return {};
    const GLayerEntity &layer = m_drawing->layers().at(m_layerNames.at(index.row()));
    if (role == Qt::DisplayRole) {
        switch (index.column()) {
        case 0: return layer.name;
        case 1: return aciColor(layer.colorIndex).name(QColor::HexRgb).toUpper();
        case 2: return layer.lineWidth <= 0.0 ? tr("默认") : QString::number(layer.lineWidth);
        case 3: return layer.lineTypeName;
        default: return {};
        }
    }
    if (role == Qt::DecorationRole && index.column() == 1)
        return aciColor(layer.colorIndex);
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
    if (!m_drawing || role != Qt::CheckStateRole || index.column() != 4)
        return false;
    const std::string &layerName = m_layerNames.at(index.row());
    const bool visible = value.toInt() == Qt::Checked;
    if (!m_drawing->setLayerVisible(layerName, visible))
        return false;
    emit dataChanged(index, index, {Qt::CheckStateRole});
    return true;
}

} // namespace totcad
