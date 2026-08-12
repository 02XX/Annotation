#include "Controller/GDocumentController.hpp"

#include "DXF/GDXFParser.hpp"
#include "IO/GAnnotationSerializer.hpp"
#include "IO/GRecentFileManager.hpp"
#include "Model/Annotation/GAnnotationDocument.hpp"
#include "Model/Entities/GDocumentEntity.hpp"

#include <QFileInfo>
#include <QUndoStack>

namespace totcad {

GDocumentController::GDocumentController(GDocumentEntity *cadDocument,
                                         GAnnotationDocument *annotationDocument,
                                         QUndoStack *undoStack,
                                         QObject *parent)
    : QObject(parent),
      m_cadDocument(cadDocument),
      m_annotationDocument(annotationDocument),
      m_undoStack(undoStack)
{
    connect(m_undoStack, &QUndoStack::cleanChanged, this, [this](bool clean) {
        if (clean)
            m_annotationDocument->setClean();
    });
}

bool GDocumentController::open(const QString &filePath, QString *errorMessage)
{
    GDocumentEntity parsedCadDocument;
    if (!GDXFParser{}.parseFile(filePath, parsedCadDocument, errorMessage))
        return false;

    const QFileInfo dxfInfo(filePath);
    const QString annotationPath = dxfInfo.absolutePath() + QLatin1Char('/')
                                   + dxfInfo.completeBaseName() + QStringLiteral(".json");
    GAnnotationDocument parsedAnnotationDocument;
    if (QFileInfo::exists(annotationPath)
        && !GAnnotationSerializer{}.load(annotationPath, parsedAnnotationDocument, errorMessage)) {
        return false;
    }
    m_cadDocument->replaceWith(parsedCadDocument);
    m_annotationDocument->restore(parsedAnnotationDocument.snapshot(), false);
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
