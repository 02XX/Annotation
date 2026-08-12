#include "Controller/GDocumentController.hpp"

#include "DXF/GDXFParser.hpp"
#include "IO/GAnnotationSerializer.hpp"
#include "IO/GRecentFileManager.hpp"
#include "Model/Annotation/GAnnotationDocument.hpp"
#include "Model/CAD/GCADDocument.hpp"

#include <QFileInfo>
#include <QUndoStack>

namespace totcad {

GDocumentController::GDocumentController(GCADDocument *cadDocument,
                                         GAnnotationDocument *annotationDocument,
                                         QUndoStack *undoStack,
                                         QObject *parent)
    : QObject(parent),
      m_cadDocument(cadDocument),
      m_annotationDocument(annotationDocument),
      m_undoStack(undoStack)
{
}

bool GDocumentController::open(const QString &filePath, QString *errorMessage)
{
    if (!GDXFParser{}.parseFile(filePath, *m_cadDocument, errorMessage))
        return false;

    m_annotationDocument->clear();
    const QString annotationPath = annotationFilePath();
    if (QFileInfo::exists(annotationPath)
        && !GAnnotationSerializer{}.load(annotationPath, *m_annotationDocument, errorMessage)) {
        m_cadDocument->clear();
        return false;
    }
    m_annotationDocument->setClean();
    m_undoStack->clear();
    m_undoStack->setClean();
    GRecentFileManager{}.addFile(filePath);
    emit documentOpened(filePath);
    return true;
}

bool GDocumentController::save(QString *errorMessage)
{
    if (!hasDocument()) {
        if (errorMessage)
            *errorMessage = tr("当前没有打开的 DXF 文件");
        return false;
    }
    if (!GAnnotationSerializer{}.save(annotationFilePath(), *m_annotationDocument, errorMessage))
        return false;
    m_annotationDocument->setClean();
    m_undoStack->setClean();
    emit documentSaved(annotationFilePath());
    return true;
}

void GDocumentController::close()
{
    m_cadDocument->clear();
    m_annotationDocument->clear();
    m_undoStack->clear();
    emit documentClosed();
}

bool GDocumentController::hasDocument() const
{
    return !m_cadDocument->sourcePath.isEmpty();
}

bool GDocumentController::isModified() const
{
    return hasDocument() && (!m_undoStack->isClean() || m_annotationDocument->isDirty());
}

QString GDocumentController::annotationFilePath() const
{
    const QFileInfo info(m_cadDocument->sourcePath);
    return info.absolutePath() + QLatin1Char('/') + info.completeBaseName() + QStringLiteral(".json");
}

} // namespace totcad
