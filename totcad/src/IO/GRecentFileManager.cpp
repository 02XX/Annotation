#include "IO/GRecentFileManager.hpp"

#include <QFileInfo>
#include <QSettings>

namespace totcad {

namespace {
const QString key = QStringLiteral("recentFiles");
}

QStringList GRecentFileManager::files() const
{
    return QSettings{}.value(key).toStringList();
}

void GRecentFileManager::addFile(const QString &filePath)
{
    const QString canonical = QFileInfo(filePath).absoluteFilePath();
    QStringList values = files();
    values.removeAll(canonical);
    values.prepend(canonical);
    while (values.size() > MaximumFiles)
        values.removeLast();
    QSettings{}.setValue(key, values);
}

void GRecentFileManager::removeFile(const QString &filePath)
{
    QStringList values = files();
    values.removeAll(QFileInfo(filePath).absoluteFilePath());
    QSettings{}.setValue(key, values);
}

void GRecentFileManager::clear()
{
    QSettings{}.remove(key);
}

} // namespace totcad
