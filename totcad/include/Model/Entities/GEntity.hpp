#pragma once

#include <QColor>
#include <QRectF>
#include <QString>
#include <cstdint>

namespace totcad
{
template <typename TKey> class IEntity
{
  public:
    virtual ~IEntity() = default;
};

template <typename TKey> class Entity : public IEntity<TKey>
{
  private:
    TKey m_id{};

  public:
    explicit Entity(const TKey &id) : m_id(id)
    {
    }
    const TKey &id() const
    {
        return m_id;
    }

    void setId(const TKey &id)
    {
        m_id = id;
    }
};

using EntityID = uint64_t;

class GEntity : public Entity<EntityID>
{
  public:
    explicit GEntity(const EntityID &id);
    ~GEntity() override;

    QString layerName{QStringLiteral("0")};
    QString lineTypeName{QStringLiteral("BYLAYER")};
    int colorIndex{256};
    QColor trueColor;
    bool paperSpace{false};

    virtual QRectF bounds() const;
};
} // namespace totcad
