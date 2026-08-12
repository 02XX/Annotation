#pragma once

#include <QStringList>

namespace totcad {

class GRecentFileManager
{
public:
    QStringList files() const;
    void addFile(const QString &filePath);
    void removeFile(const QString &filePath);
    void clear();

private:
    static constexpr int MaximumFiles = 5;
};

} // namespace totcad
