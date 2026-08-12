#pragma once

#include "Model/Entities/GBlockEntity.hpp"
#include "Model/Entities/GLayerEntity.hpp"
#include "Model/Entities/GLineTypeEntity.hpp"
#include "Model/Entities/GSpaceEntity.hpp"

#include <QHash>
#include <QObject>
#include <memory>
#include <vector>

namespace totcad {

class GDocumentEntity final : public QObject
{
    Q_OBJECT
public:
    explicit GDocumentEntity(QObject *parent = nullptr);

    void clear();
    void replaceWith(GDocumentEntity &other);
    void addEntity(std::shared_ptr<GEntity> entity);
    void addLayer(GLayerEntity layer);
    void addLineType(GLineTypeEntity lineType);
    void addBlock(GBlockEntity block);

    const std::vector<std::shared_ptr<GEntity>> &entities() const noexcept { return m_entities; }
    const QHash<QString, GLayerEntity> &layers() const noexcept { return m_layers; }
    QHash<QString, GLayerEntity> &layers() noexcept { return m_layers; }
    const QHash<QString, GLineTypeEntity> &lineTypes() const noexcept { return m_lineTypes; }
    const QHash<QString, GBlockEntity> &blocks() const noexcept { return m_blocks; }
    const GBlockEntity *block(const QString &name) const;
    std::shared_ptr<GEntity> entity(EntityID id) const;
    QRectF bounds(bool includePaperSpace = false) const;
    int entityCount() const noexcept { return static_cast<int>(m_entities.size()); }

    QString sourcePath;
    GSpaceEntity modelSpace{QStringLiteral("Model"), false, {}};
    GSpaceEntity paperSpace{QStringLiteral("Paper"), true, {}};

signals:
    void layerVisibilityChanged(const QString &layerName, bool visible);

private:
    std::vector<std::shared_ptr<GEntity>> m_entities;
    QHash<EntityID, std::shared_ptr<GEntity>> m_entityById;
    QHash<QString, GLayerEntity> m_layers;
    QHash<QString, GLineTypeEntity> m_lineTypes;
    QHash<QString, GBlockEntity> m_blocks;
};

} // namespace totcad
