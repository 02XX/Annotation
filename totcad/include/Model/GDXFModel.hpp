#pragma once

#include "Model/Entities/GBlockEntity.hpp"
#include "Model/Entities/GLayerEntity.hpp"
#include "Model/Entities/GLineTypeEntity.hpp"
#include "Model/Entities/GSpaceEntity.hpp"

#include <QObject>
#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>

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
    bool setLayerVisible(std::string_view name, bool visible);

    const std::vector<std::shared_ptr<GEntity>> &entities() const noexcept
    {
        return m_entities;
    }
    const std::unordered_map<std::string, GLayerEntity> &layers() const noexcept
    {
        return m_layers;
    }
    const std::unordered_map<std::string, GLineTypeEntity> &lineTypes() const noexcept
    {
        return m_lineTypes;
    }
    const std::unordered_map<std::string, GBlockEntity> &blocks() const noexcept
    {
        return m_blocks;
    }
    const GBlockEntity *block(std::string_view name) const noexcept;
    std::shared_ptr<GEntity> entity(EntityID id) const;

    const GSpaceEntity &modelSpace() const noexcept
    {
        return m_modelSpace;
    }
    const GSpaceEntity &paperSpace() const noexcept
    {
        return m_paperSpace;
    }
    const std::string &sourcePath() const noexcept
    {
        return m_sourcePath;
    }
    void setSourcePath(std::string path)
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
    std::vector<std::shared_ptr<GEntity>> m_entities;
    std::unordered_map<EntityID, std::shared_ptr<GEntity>> m_entityById;
    std::unordered_map<std::string, GLayerEntity> m_layers;
    std::unordered_map<std::string, GLineTypeEntity> m_lineTypes;
    std::unordered_map<std::string, GBlockEntity> m_blocks;
    GSpaceEntity m_modelSpace{QStringLiteral("Model"), false, {}};
    GSpaceEntity m_paperSpace{QStringLiteral("Paper"), true, {}};
    std::string m_sourcePath;
};

} // namespace totcad
