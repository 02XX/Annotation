#pragma once

#include <QAbstractTableModel>
#include <string>
#include <vector>

namespace totcad {

class GDXFModel;

class GLayerTableModel final : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit GLayerTableModel(QObject *parent = nullptr);
    void setDrawing(GDXFModel *drawing);
    int rowCount(const QModelIndex &parent = {}) const override;
    int columnCount(const QModelIndex &parent = {}) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;

private:
    GDXFModel *m_drawing{nullptr};
    std::vector<std::string> m_layerNames;
};

} // namespace totcad
