#include "Model/Entities/GDocumentEntity.hpp"

namespace totcad {

GDocumentEntity::GDocumentEntity(QObject *parent) : QObject(parent)
{
    addLayer(GLayerEntity{});
    addLineType(GLineTypeEntity{});
}

void GDocumentEntity::clear()
{
    m_entities.clear();
    m_entityById.clear();
    m_layers.clear();
    m_lineTypes.clear();
    m_blocks.clear();
    modelSpace.entityIds.clear();
    paperSpace.entityIds.clear();
    sourcePath.clear();
    addLayer(GLayerEntity{});
    addLineType(GLineTypeEntity{});
}

void GDocumentEntity::replaceWith(GDocumentEntity &other)
{
    m_entities = std::move(other.m_entities);
    m_entityById = std::move(other.m_entityById);
    m_layers = std::move(other.m_layers);
    m_lineTypes = std::move(other.m_lineTypes);
    m_blocks = std::move(other.m_blocks);
    modelSpace = std::move(other.modelSpace);
    paperSpace = std::move(other.paperSpace);
    sourcePath = std::move(other.sourcePath);
}

void GDocumentEntity::addEntity(std::shared_ptr<GEntity> entity)
{
    if (!entity)
        return;
    m_entityById.insert(entity->id(), entity);
    (entity->paperSpace ? paperSpace : modelSpace).entityIds.append(entity->id());
    m_entities.push_back(std::move(entity));
}

void GDocumentEntity::addLayer(GLayerEntity layer)
{
    m_layers.insert(layer.name, std::move(layer));
}

void GDocumentEntity::addLineType(GLineTypeEntity lineType)
{
    m_lineTypes.insert(lineType.name, std::move(lineType));
}

void GDocumentEntity::addBlock(GBlockEntity block)
{
    m_blocks.insert(block.name, std::move(block));
}

const GBlockEntity *GDocumentEntity::block(const QString &name) const
{
    const auto it = m_blocks.constFind(name);
    return it == m_blocks.cend() ? nullptr : &it.value();
}

std::shared_ptr<GEntity> GDocumentEntity::entity(EntityID id) const
{
    return m_entityById.value(id);
}

QRectF GDocumentEntity::bounds(bool includePaperSpace) const
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
