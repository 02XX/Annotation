#include "Model/CAD/GCADDocument.hpp"

namespace totcad {

GCADDocument::GCADDocument(QObject *parent) : QObject(parent)
{
    addLayer(GCADLayer{});
    addLineType(GCADLineType{});
}

void GCADDocument::clear()
{
    m_entities.clear();
    m_entityById.clear();
    m_layers.clear();
    m_lineTypes.clear();
    m_blocks.clear();
    modelSpace.entityIds.clear();
    paperSpace.entityIds.clear();
    sourcePath.clear();
    addLayer(GCADLayer{});
    addLineType(GCADLineType{});
}

void GCADDocument::replaceWith(GCADDocument &other)
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

void GCADDocument::addEntity(std::shared_ptr<GCADEntity> entity)
{
    if (!entity)
        return;
    m_entityById.insert(entity->id(), entity);
    (entity->paperSpace ? paperSpace : modelSpace).entityIds.append(entity->id());
    m_entities.push_back(std::move(entity));
}

void GCADDocument::addLayer(GCADLayer layer)
{
    m_layers.insert(layer.name, std::move(layer));
}

void GCADDocument::addLineType(GCADLineType lineType)
{
    m_lineTypes.insert(lineType.name, std::move(lineType));
}

void GCADDocument::addBlock(GCADBlock block)
{
    m_blocks.insert(block.name, std::move(block));
}

const GCADBlock *GCADDocument::block(const QString &name) const
{
    const auto it = m_blocks.constFind(name);
    return it == m_blocks.cend() ? nullptr : &it.value();
}

std::shared_ptr<GCADEntity> GCADDocument::entity(const QString &id) const
{
    return m_entityById.value(id);
}

QRectF GCADDocument::bounds(bool includePaperSpace) const
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
