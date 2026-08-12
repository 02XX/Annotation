#pragma once

#include "Model/CAD/GCADBlock.hpp"
#include "Model/CAD/GCADLayer.hpp"
#include "Model/CAD/GCADLineType.hpp"
#include "Model/CAD/GCADSpace.hpp"

#include <QHash>
#include <QObject>
#include <memory>
#include <vector>

namespace totcad {

class GCADDocument final : public QObject
{
    Q_OBJECT
public:
    explicit GCADDocument(QObject *parent = nullptr);

    void clear();
    void replaceWith(GCADDocument &other);
    void addEntity(std::shared_ptr<GCADEntity> entity);
    void addLayer(GCADLayer layer);
    void addLineType(GCADLineType lineType);
    void addBlock(GCADBlock block);

    const std::vector<std::shared_ptr<GCADEntity>> &entities() const noexcept { return m_entities; }
    const QHash<QString, GCADLayer> &layers() const noexcept { return m_layers; }
    QHash<QString, GCADLayer> &layers() noexcept { return m_layers; }
    const QHash<QString, GCADLineType> &lineTypes() const noexcept { return m_lineTypes; }
    const QHash<QString, GCADBlock> &blocks() const noexcept { return m_blocks; }
    const GCADBlock *block(const QString &name) const;
    std::shared_ptr<GCADEntity> entity(const QString &id) const;
    QRectF bounds(bool includePaperSpace = false) const;
    int entityCount() const noexcept { return static_cast<int>(m_entities.size()); }

    QString sourcePath;
    GCADSpace modelSpace{QStringLiteral("Model"), false, {}};
    GCADSpace paperSpace{QStringLiteral("Paper"), true, {}};

signals:
    void layerVisibilityChanged(const QString &layerName, bool visible);

private:
    std::vector<std::shared_ptr<GCADEntity>> m_entities;
    QHash<QString, std::shared_ptr<GCADEntity>> m_entityById;
    QHash<QString, GCADLayer> m_layers;
    QHash<QString, GCADLineType> m_lineTypes;
    QHash<QString, GCADBlock> m_blocks;
};

} // namespace totcad
