#include "DXF/GDXFReader.hpp"

#include <QFile>
#include <QTextCodec>
#include <QTextStream>

namespace totcad {

bool GDXFReader::readFile(const QString &filePath, QVector<GDXFGroup> &groups, QString *errorMessage) const
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        if (errorMessage)
            *errorMessage = QObject::tr("无法读取文件：%1").arg(file.errorString());
        return false;
    }

    QTextStream stream(&file);
    stream.setCodec(QTextCodec::codecForLocale());
    groups.clear();
    int lineNumber = 0;
    while (!stream.atEnd()) {
        const QString codeLine = stream.readLine();
        ++lineNumber;
        if (stream.atEnd() && codeLine.trimmed().isEmpty())
            break;
        if (stream.atEnd()) {
            if (errorMessage)
                *errorMessage = QObject::tr("DXF 第 %1 行之后缺少组值").arg(lineNumber);
            return false;
        }
        const QString valueLine = stream.readLine();
        ++lineNumber;
        bool ok = false;
        const int code = codeLine.trimmed().toInt(&ok);
        if (!ok) {
            if (errorMessage)
                *errorMessage = QObject::tr("DXF 第 %1 行的组码无效：%2").arg(lineNumber - 1).arg(codeLine);
            return false;
        }
        groups.append({code, valueLine.trimmed(), lineNumber - 1});
    }
    return true;
}

} // namespace totcad
