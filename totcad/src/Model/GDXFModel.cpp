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
    return m_entities.isEmpty() && m_blocks.isEmpty();
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
    if (!entity || m_entityById.contains(entity->id()))
        return false;
    m_entityById.insert(entity->id(), entity);
    (entity->paperSpace ? m_paperSpace : m_modelSpace).entityIds.push_back(entity->id());
    m_entities.push_back(std::move(entity));
    return true;
}

void GDXFModel::addLayer(GLayerEntity layer)
{
    const QString name = layer.name;
    m_layers.insert(name, std::move(layer));
}

void GDXFModel::addLineType(GLineTypeEntity lineType)
{
    const QString name = lineType.name;
    m_lineTypes.insert(name, std::move(lineType));
}

void GDXFModel::addBlock(GBlockEntity block)
{
    const QString name = block.name;
    m_blocks.insert(name, std::move(block));
}

bool GDXFModel::setLayerVisible(const QString &name, bool visible)
{
    auto layer = m_layers.find(name);
    if (layer == m_layers.end() || layer.value().visible == visible)
        return false;
    layer.value().visible = visible;
    emit layerVisibilityChanged();
    return true;
}

const GBlockEntity *GDXFModel::block(const QString &name) const noexcept
{
    const auto found = m_blocks.constFind(name);
    return found == m_blocks.cend() ? nullptr : &found.value();
}

std::shared_ptr<GEntity> GDXFModel::entity(EntityID id) const
{
    return m_entityById.value(id);
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
