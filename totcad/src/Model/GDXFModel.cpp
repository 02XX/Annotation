#include "Model/GDXFModel.hpp"

#include <utility>

namespace totcad {

GDXFModel::GDXFModel(QObject *parent) : QObject(parent)
{
    addLayer(GLayerEntity{});
    addLineType(GLineTypeEntity{});
}

void GDXFModel::clear()
{
    m_entities.clear();
    m_entityById.clear();
    m_layers.clear();
    m_lineTypes.clear();
    m_blocks.clear();
    m_modelSpace.entityIds.clear();
    m_paperSpace.entityIds.clear();
    m_sourcePath.clear();
    addLayer(GLayerEntity{});
    addLineType(GLineTypeEntity{});
    emit modelReset();
}

bool GDXFModel::isEmpty() const noexcept
{
    return m_entities.empty() && m_blocks.empty();
}

void GDXFModel::replaceWith(GDXFModel &&other)
{
    m_entities = std::move(other.m_entities);
    m_entityById = std::move(other.m_entityById);
    m_layers = std::move(other.m_layers);
    m_lineTypes = std::move(other.m_lineTypes);
    m_blocks = std::move(other.m_blocks);
    m_modelSpace = std::move(other.m_modelSpace);
    m_paperSpace = std::move(other.m_paperSpace);
    m_sourcePath = std::move(other.m_sourcePath);
    emit modelReset();
}

bool GDXFModel::addEntity(std::shared_ptr<GEntity> entity)
{
    if (!entity || m_entityById.find(entity->id()) != m_entityById.end())
        return false;
    m_entityById.emplace(entity->id(), entity);
    (entity->paperSpace ? m_paperSpace : m_modelSpace).entityIds.push_back(entity->id());
    m_entities.push_back(std::move(entity));
    return true;
}

void GDXFModel::addLayer(GLayerEntity layer)
{
    const std::string name = layer.name.toStdString();
    m_layers.insert_or_assign(name, std::move(layer));
}

void GDXFModel::addLineType(GLineTypeEntity lineType)
{
    const std::string name = lineType.name.toStdString();
    m_lineTypes.insert_or_assign(name, std::move(lineType));
}

void GDXFModel::addBlock(GBlockEntity block)
{
    const std::string name = block.name.toStdString();
    m_blocks.insert_or_assign(name, std::move(block));
}

bool GDXFModel::setLayerVisible(std::string_view name, bool visible)
{
    auto layer = m_layers.find(std::string(name));
    if (layer == m_layers.end() || layer->second.visible == visible)
        return false;
    layer->second.visible = visible;
    emit layerVisibilityChanged();
    return true;
}

const GBlockEntity *GDXFModel::block(std::string_view name) const noexcept
{
    const auto found = m_blocks.find(std::string(name));
    return found == m_blocks.end() ? nullptr : &found->second;
}

std::shared_ptr<GEntity> GDXFModel::entity(EntityID id) const
{
    const auto found = m_entityById.find(id);
    return found == m_entityById.end() ? nullptr : found->second;
}

QRectF GDXFModel::bounds(bool includePaperSpace) const
{
    QRectF result;
    for (const auto &entityValue : m_entities) {
        if (!includePaperSpace && entityValue->paperSpace)
            continue;
        const QRectF entityBounds = entityValue->bounds();
        result = result.isNull() ? entityBounds : result.united(entityBounds);
    }
    return result;
}

} // namespace totcad
