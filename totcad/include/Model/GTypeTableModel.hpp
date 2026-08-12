#pragma once

#include <QAbstractTableModel>

namespace totcad {

class GAnnotationDocument;

class GTypeTableModel final : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit GTypeTableModel(GAnnotationDocument *document, QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = {}) const override;
    int columnCount(const QModelIndex &parent = {}) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    QString typeId(int row) const;

private:
    GAnnotationDocument *m_document;
};

} // namespace totcad
