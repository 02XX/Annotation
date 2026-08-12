#pragma once

#include <QAbstractItemModel>

namespace totcad {

class GAnnotationDocument;

class GInstanceTreeModel final : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit GInstanceTreeModel(GAnnotationDocument *document, QObject *parent = nullptr);
    QModelIndex index(int row, int column, const QModelIndex &parent = {}) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    int rowCount(const QModelIndex &parent = {}) const override;
    int columnCount(const QModelIndex &parent = {}) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;

    void setCurrentType(const QString &typeId);
    QString currentType() const { return m_typeId; }
    QString instanceId(const QModelIndex &index) const;

private:
    GAnnotationDocument *m_document;
    QString m_typeId;
};

} // namespace totcad
