#pragma once

#include "Model/Entities/GBlockEntity.hpp"
#include "Model/Entities/GLayerEntity.hpp"
#include "Model/Entities/GLineTypeEntity.hpp"
#include "Model/Entities/GSpaceEntity.hpp"

#include <QHash>
#include <QObject>
#include <QString>
#include <QVector>
#include <memory>
#include <utility>

namespace totcad
{

// 从 DXFDocument 构造出的运行时图纸模型，供界面查询和显示。
//
// 职责：
// 1. 拥有实体、图层、线型、块和空间；
// 2. 维护实体 ID 索引和实体所属空间；
// 3. 提供整张图纸的查询、边界计算、替换与清空操作。
//
// 不负责 DXF 文本解析、图形渲染和标注数据。
class GDXFModel final : public QObject
{
    Q_OBJECT
public:
    explicit GDXFModel(QObject *parent = nullptr);

    void clear();
    bool isEmpty() const noexcept;
    void replaceWith(GDXFModel &&other);

    bool addEntity(std::shared_ptr<GEntity> entity);
    void addLayer(GLayerEntity layer);
    void addLineType(GLineTypeEntity lineType);
    void addBlock(GBlockEntity block);
    bool setLayerVisible(const QString &name, bool visible);

    const QVector<std::shared_ptr<GEntity>> &entities() const noexcept
    {
        return m_entities;
    }
    const QHash<QString, GLayerEntity> &layers() const noexcept
    {
        return m_layers;
    }
    const QHash<QString, GLineTypeEntity> &lineTypes() const noexcept
    {
        return m_lineTypes;
    }
    const QHash<QString, GBlockEntity> &blocks() const noexcept
    {
        return m_blocks;
    }
    const GBlockEntity *block(const QString &name) const noexcept;
    std::shared_ptr<GEntity> entity(EntityID id) const;

    const GSpaceEntity &modelSpace() const noexcept
    {
        return m_modelSpace;
    }
    const GSpaceEntity &paperSpace() const noexcept
    {
        return m_paperSpace;
    }
    const QString &sourcePath() const noexcept
    {
        return m_sourcePath;
    }
    void setSourcePath(QString path)
    {
        m_sourcePath = std::move(path);
    }

    QRectF bounds(bool includePaperSpace = false) const;
    std::size_t entityCount() const noexcept
    {
        return m_entities.size();
    }

signals:
    void modelReset();
    void layerVisibilityChanged();

private:
    QVector<std::shared_ptr<GEntity>> m_entities;
    QHash<EntityID, std::shared_ptr<GEntity>> m_entityById;
    QHash<QString, GLayerEntity> m_layers;
    QHash<QString, GLineTypeEntity> m_lineTypes;
    QHash<QString, GBlockEntity> m_blocks;
    GSpaceEntity m_modelSpace{QStringLiteral("Model"), false, {}};
    GSpaceEntity m_paperSpace{QStringLiteral("Paper"), true, {}};
    QString m_sourcePath;
};

} // namespace totcad
